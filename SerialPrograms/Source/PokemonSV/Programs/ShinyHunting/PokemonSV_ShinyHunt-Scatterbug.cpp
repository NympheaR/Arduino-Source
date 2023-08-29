/*  Shiny Hunt - Scatterbug
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <atomic>
#include <sstream>
#include "Common/Cpp/PrettyPrint.h"
#include "CommonFramework/GlobalSettingsPanel.h"
#include "CommonFramework/Exceptions/ProgramFinishedException.h"
#include "CommonFramework/Exceptions/OperationFailedException.h"
#include "CommonFramework/Exceptions/FatalProgramException.h"
#include "CommonFramework/Notifications/ProgramNotifications.h"
#include "CommonFramework/InferenceInfra/InferenceRoutines.h"
#include "CommonFramework/Tools/StatsTracking.h"
#include "CommonFramework/Tools/VideoResolutionCheck.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "Pokemon/Pokemon_Strings.h"
#include "PokemonSV/PokemonSV_Settings.h"
#include "PokemonSV/Inference/Overworld/PokemonSV_LetsGoHpReader.h"
#include "PokemonSV/Inference/Boxes/PokemonSV_IVCheckerReader.h"
#include "PokemonSV/Inference/Battles/PokemonSV_EncounterWatcher.h"
#include "PokemonSV/Programs/PokemonSV_AreaZero.h"
#include "PokemonSV/Programs/PokemonSV_Battles.h"
#include "PokemonSV/Programs/PokemonSV_GameEntry.h"
#include "PokemonSV/Programs/PokemonSV_Navigation.h"
#include "PokemonSV/Programs/PokemonSV_SaveGame.h"
#include "PokemonSV_LetsGoTools.h"
#include "PokemonSV_ShinyHunt-Scatterbug.h"

//#include <iostream>
//using std::cout;
//using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

using namespace Pokemon;



ShinyHuntScatterbug_Descriptor::ShinyHuntScatterbug_Descriptor()
    : SingleSwitchProgramDescriptor(
        "PokemonSV:ShinyHuntScatterbug",
        STRING_POKEMON + " SV", "Shiny Hunt - Scatterbug",
        "ComputerControl/blob/master/Wiki/Programs/PokemonSV/ShinyHunt-Scatterbug.md",
        "Shiny hunt Scatterbug.",
        FeedbackType::REQUIRED,
        AllowCommandsWhenRunning::DISABLE_COMMANDS,
        PABotBaseLevel::PABOTBASE_12KB
    )
{}
struct ShinyHuntScatterbug_Descriptor::Stats : public LetsGoEncounterBotStats{
    Stats()
        : m_sandwiches(m_stats["Sandwiches"])
        , m_autoheals(m_stats["Auto Heals"])
        , m_path_resets(m_stats["Path Resets"])
        , m_game_resets(m_stats["Game Resets"])
        , m_errors(m_stats["Errors"])
    {
        m_display_order.insert(m_display_order.begin() + 2, {"Sandwiches", HIDDEN_IF_ZERO});
        m_display_order.insert(m_display_order.begin() + 3, {"Auto Heals", HIDDEN_IF_ZERO});
        m_display_order.insert(m_display_order.begin() + 4, {"Path Resets", HIDDEN_IF_ZERO});
        m_display_order.insert(m_display_order.begin() + 5, {"Game Resets", HIDDEN_IF_ZERO});
        m_display_order.insert(m_display_order.begin() + 6, {"Errors", HIDDEN_IF_ZERO});
    }
    std::atomic<uint64_t>& m_sandwiches;
    std::atomic<uint64_t>& m_autoheals;
    std::atomic<uint64_t>& m_path_resets;
    std::atomic<uint64_t>& m_game_resets;
    std::atomic<uint64_t>& m_errors;
};
std::unique_ptr<StatsTracker> ShinyHuntScatterbug_Descriptor::make_stats() const{
    return std::unique_ptr<StatsTracker>(new Stats());
}




ShinyHuntScatterbug::ShinyHuntScatterbug()
    : LANGUAGE(
        "<b>Game Language:</b><br>Required to read " + STRING_POKEMON + " names.",
        IV_READER().languages(),
        LockWhileRunning::UNLOCKED,
        false
    )
    , GO_HOME_WHEN_DONE(true)
    , AUTO_HEAL_PERCENT(
        "<b>Auto-Heal %</b><br>Auto-heal if your HP drops below this percentage.",
        LockWhileRunning::UNLOCKED,
        75, 0, 100
    )
    , NOTIFICATION_STATUS_UPDATE("Status Update", true, false, std::chrono::seconds(3600))
    , NOTIFICATIONS({
        &NOTIFICATION_STATUS_UPDATE,
        &ENCOUNTER_BOT_OPTIONS.NOTIFICATION_NONSHINY,
        &ENCOUNTER_BOT_OPTIONS.NOTIFICATION_SHINY,
        &ENCOUNTER_BOT_OPTIONS.NOTIFICATION_CATCH_SUCCESS,
        &ENCOUNTER_BOT_OPTIONS.NOTIFICATION_CATCH_FAILED,
        &NOTIFICATION_PROGRAM_FINISH,
        &NOTIFICATION_ERROR_RECOVERABLE,
        &NOTIFICATION_ERROR_FATAL,
    })
{
    PA_ADD_OPTION(LANGUAGE);
    PA_ADD_OPTION(ENCOUNTER_BOT_OPTIONS);
    PA_ADD_OPTION(GO_HOME_WHEN_DONE);
    PA_ADD_OPTION(AUTO_HEAL_PERCENT);
    PA_ADD_OPTION(NOTIFICATIONS);
}


void ShinyHuntScatterbug::program(SingleSwitchProgramEnvironment& env, BotBaseContext& context){
    ShinyHuntScatterbug_Descriptor::Stats& stats = env.current_stats<ShinyHuntScatterbug_Descriptor::Stats>();

    //  Connect the controller.
    pbf_press_button(context, BUTTON_LCLICK, 10, 0);

    assert_16_9_720p_min(env.logger(), env.console);

    m_iterations = 0;

    LetsGoHpWatcher hp_watcher(COLOR_RED);
    m_hp_watcher = &hp_watcher;

    DiscontiguousTimeTracker time_tracker;
    m_time_tracker = &time_tracker;

    LetsGoEncounterBotTracker encounter_tracker(
        env, env.console, context,
        stats,
        LANGUAGE
    );
    m_encounter_tracker = &encounter_tracker;

    m_pending_save = false;

    // Which path to choose starting at the PokeCenter.
    size_t path_id = 0;
    const size_t num_paths = 3;

    //  This is the outer-most program loop that wraps all logic with the
    //  battle menu detector. If at any time you detect a battle menu, you break
    //  all the way out here to handle the encounter. This is needed because you
    //  can run into pokemon while moving around.
    m_consecutive_failures = 0;
    while (true){
        env.console.log("Starting encounter loop...", COLOR_PURPLE);
        EncounterWatcher encounter_watcher(env.console, COLOR_RED);
        run_until(
            env.console, context,
            [&](BotBaseContext& context){
                run_iteration(env, context, path_id);
            },
            {
                static_cast<VisualInferenceCallback&>(encounter_watcher),
                static_cast<AudioInferenceCallback&>(encounter_watcher),
                hp_watcher,
            }
        );
        encounter_watcher.throw_if_no_sound();

        env.console.log("Detected battle.", COLOR_PURPLE);
        try{
            bool should_save = encounter_tracker.process_battle(encounter_watcher, ENCOUNTER_BOT_OPTIONS);
            if (should_save){
                m_pending_save = should_save;
            }
        }catch (ProgramFinishedException&){
            GO_HOME_WHEN_DONE.run_end_of_program(context);
            throw;
        }
        // Use map to fly back to the pokecenter
        reset_to_pokecenter(env, context);

        path_id = (path_id + 1) % num_paths;
    }
}

// One iteration of the hunt: 
// start at Mesagoza South Gate pokecenter, go out and use Let's Go to battle Scatterbug, 
void ShinyHuntScatterbug::run_iteration(SingleSwitchProgramEnvironment& env, BotBaseContext& context, size_t path_id){
    auto& console = env.console;
    // Orient camera to look at same direction as player character
    // This is needed because when save-load the game, the camera is reset
    // to this location.
    pbf_press_button(context, BUTTON_L, 50, 40);

    if (path_id == 1){
        // move west
        pbf_move_left_joystick(context, 255, 128, 100, 20);
        // Align camera
        pbf_press_button(context, BUTTON_L, 50, 40);
    } else if (path_id == 2){
        // move east
        pbf_move_left_joystick(context, 0, 128, 100, 20);
        // Align camera
        pbf_press_button(context, BUTTON_L, 50, 40);
    }

    const bool throw_ball_if_bubble = true;

    for(int i = 0; i < 5; i++){
        use_lets_go_to_clear_in_front(console, context, *m_encounter_tracker, throw_ball_if_bubble, [&](BotBaseContext& context){
            // Do the following movement while the Let's Go pokemon clearing wild pokemon.
            // Slowly Moving forward
            pbf_move_left_joystick(context, 128, 105, 800, 0);
        });
    }
}

// Open map and teleport back to town pokecenter to reset the hunting path.
void ShinyHuntScatterbug::reset_to_pokecenter(SingleSwitchProgramEnvironment& env, BotBaseContext& context){
    open_map_from_overworld(env.program_info(), env.console, context);
    fly_to_closest_pokecenter_on_map(env.program_info(), env.console, context);
}


}
}
}
