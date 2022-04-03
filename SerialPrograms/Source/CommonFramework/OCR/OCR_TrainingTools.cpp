/*  Training Tools
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QDirIterator>
#include "Common/Cpp/PrettyPrint.h"
#include "Common/Cpp/ParallelTaskRunner.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/GlobalSettingsPanel.h"
#include "CommonFramework/OCR/OCR_RawOCR.h"
#include "CommonFramework/OCR/OCR_Filtering.h"
#include "CommonFramework/OCR/OCR_StringNormalization.h"
#include "OCR_SmallDictionaryMatcher.h"
#include "OCR_LargeDictionaryMatcher.h"
#include "OCR_TrainingTools.h"

namespace PokemonAutomation{
namespace OCR{



std::string extract_name(const QString& filename){
    std::string name = filename.toUtf8().data();
    while (!name.empty()){
        char ch = name.back();
        name.pop_back();
        if (ch == '.'){
            break;
        }
    }
    while (!name.empty()){
        char ch = name.back();
        name.pop_back();
        if (ch == '-'){
            break;
        }
    }
    while (!name.empty()){
        char ch = name.back();
        name.pop_back();
        if (ch == '-'){
            break;
        }
    }
    return name;
}



TrainingSession::TrainingSession(
    LoggerQt& logger, CancellableScope& scope,
    const QString& training_data_directory
)
    : m_logger(logger)
    , m_scope(scope)
    , m_directory(TRAINING_PATH() + training_data_directory)
    , m_total_samples(0)
{
    if (!m_directory.isEmpty() && m_directory.back() != '/' && m_directory.back() != '\\'){
        m_directory += "/";
    }

    logger.log("Parsing training data in: " + m_directory);

    QDirIterator iter(m_directory, QDir::AllDirs);
    while (iter.hasNext()){
        iter.next();
        QString sample_directory = iter.fileName() + "/";
        for (size_t c = 1; c < (size_t)Language::EndOfList; c++){
            Language language = (Language)c;
            const std::string& code = language_data(language).code;
            QString folder = sample_directory + QString::fromStdString(code) + "/";
            QDirIterator iter1(m_directory + folder, QStringList() << "*.png", QDir::Files);
            while (iter1.hasNext()){
                iter1.next();
//                QString file = iter.next();
                m_samples[language].emplace_back(
                    TrainingSample{
                        OCR::extract_name(iter1.fileName()),
                        folder + iter1.fileName()
                    }
                );
                m_total_samples++;

                scope.throw_if_cancelled();
            }
        }
        scope.throw_if_cancelled();
    }

    logger.log(
        "Parsing Complete: Languages = " + tostr_u_commas(m_samples.size()) +
        ", Samples = " + tostr_u_commas(m_total_samples)
    );
}


void TrainingSession::generate_small_dictionary(
    const QString& ocr_json_file,
    const QString& output_json_file,
    bool incremental,
    size_t threads
){
    m_logger.log("Generating OCR Data...");

    OCR::SmallDictionaryMatcher baseline(ocr_json_file, !incremental);
    OCR::SmallDictionaryMatcher trained(ocr_json_file, !incremental);

    ParallelTaskRunner task_runner(
        [](){ GlobalSettings::instance().COMPUTE_PRIORITY0.set_on_this_thread(); },
        0, threads
    );

    std::atomic<size_t> matched(0);
    std::atomic<size_t> failed(0);
    for (const auto& language : m_samples){
        const LanguageData& language_info = language_data(language.first);
        m_logger.log("Starting Language: " + language_info.name);
//        cout << (int)item.first << " : " << item.second.size() << endl;
        for (const TrainingSample& sample : language.second){
            task_runner.dispatch([&]{
                QImage image(m_directory + sample.filepath);
                if (image.isNull()){
                    m_logger.log("Skipping: " + sample.filepath);
                    return;
                }
                OCR::make_OCR_filter(image).apply(image);
                QString text = OCR::ocr_read(language.first, image);
                QString normalized = OCR::normalize(text);

                OCR::StringMatchResult result = baseline.match_substring(language.first, text, LOG10P_SPREAD);

                OCR::StringMatchResult result0 = result;
                result.clear_beyond_log10p(MAX_LOG10P);

                if (result.results.empty()){
                    failed++;
                    result0.log(m_logger, MAX_LOG10P, sample.filepath);
                    trained.add_candidate(language.first, sample.token, normalized);
                    return;
                }

                for (const auto& item : result.results){
                    if (item.second.token == sample.token){
                        matched++;
                        return;
                    }
                }
            });
            m_scope.throw_if_cancelled();
        }
        task_runner.wait_for_everything();
        m_scope.throw_if_cancelled();
    }

    m_logger.log("Languages: " + tostr_u_commas(m_samples.size()));
    m_logger.log("Samples: " + tostr_u_commas(m_total_samples));
    m_logger.log("Matched: " + tostr_u_commas(matched));
    m_logger.log("Missed: " + tostr_u_commas(failed));

    trained.save(output_json_file);
}

void TrainingSession::generate_large_dictionary(
    const QString& ocr_json_directory,
    const QString& output_prefix,
    bool incremental,
    size_t threads
) const{
    m_logger.log("Generating OCR Data...");

    OCR::LargeDictionaryMatcher baseline(ocr_json_directory + output_prefix, nullptr, !incremental);
    OCR::LargeDictionaryMatcher trained(ocr_json_directory + output_prefix, nullptr, !incremental);

    ParallelTaskRunner task_runner(
        [](){ GlobalSettings::instance().COMPUTE_PRIORITY0.set_on_this_thread(); },
        0, threads
    );

    std::atomic<size_t> matched(0);
    std::atomic<size_t> failed(0);
    for (const auto& language : m_samples){
        const LanguageData& language_info = language_data(language.first);
        m_logger.log("Starting Language: " + language_info.name);
//        cout << (int)item.first << " : " << item.second.size() << endl;
        for (const TrainingSample& sample : language.second){
            task_runner.dispatch([&]{
                QImage image(m_directory + sample.filepath);
                if (image.isNull()){
                    m_logger.log("Skipping: " + sample.filepath);
                    return;
                }
                OCR::make_OCR_filter(image).apply(image);
                QString text = OCR::ocr_read(language.first, image);
                QString normalized = OCR::normalize(text);

                OCR::StringMatchResult result = baseline.match_substring(language.first, text, LOG10P_SPREAD);

                OCR::StringMatchResult result0 = result;
                result.clear_beyond_log10p(MAX_LOG10P);

                if (result.results.empty()){
                    failed++;
                    result0.log(m_logger, MAX_LOG10P, sample.filepath);
                    trained.add_candidate(language.first, sample.token, normalized);
                    return;
                }

                for (const auto& item : result.results){
                    if (item.second.token == sample.token){
                        matched++;
                        return;
                    }
                }
            });
            m_scope.throw_if_cancelled();
        }
        task_runner.wait_for_everything();

        QString json = output_prefix + QString::fromStdString(language_info.code) + ".json";
        trained.save(language.first, json);
        m_scope.throw_if_cancelled();
    }

    m_logger.log("Languages: " + tostr_u_commas(m_samples.size()));
    m_logger.log("Samples: " + tostr_u_commas(m_total_samples));
    m_logger.log("Matched: " + tostr_u_commas(matched));
    m_logger.log("Missed: " + tostr_u_commas(failed));
}






}
}

