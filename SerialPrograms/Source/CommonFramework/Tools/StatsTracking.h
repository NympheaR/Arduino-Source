/*  Stats Tracking
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_StatsTracking_H
#define PokemonAutomation_StatsTracking_H

#include <vector>
#include <string>
#include <map>
#include <atomic>

namespace PokemonAutomation{

class Logger;

class StatsTracker{
    StatsTracker(const StatsTracker&) = delete;

public:
    StatsTracker() = default;
    virtual ~StatsTracker() = default;

    virtual std::string to_str() const;

    void parse_and_append_line(const std::string& line);


protected:
    static constexpr bool HIDDEN_IF_ZERO = true;
    static constexpr bool ALWAYS_VISIBLE = false;
    struct Stat{
        std::string label;
        bool omit_if_zero;
        Stat(std::string&& p_label, bool p_omit_if_zero = ALWAYS_VISIBLE);
    };

    std::vector<Stat> m_display_order;
    std::map<std::string, std::atomic<uint64_t>> m_stats;
    std::map<std::string, std::string> m_aliases;
};


std::string stats_to_bar(
    Logger& logger,
    const StatsTracker* historical,
    const StatsTracker* current,
    const std::string& override_current = ""
);




}
#endif
