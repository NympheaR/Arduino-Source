/*  Tree Actions
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/VideoPipeline/VideoFeed.h"
#include "CommonFramework/InferenceInfra/InferenceRoutines.h"
#include "PokemonLA/Programs/PokemonLA_MountChange.h"
#include "PokemonLA/Inference/Battles/PokemonLA_BattleMenuDetector.h"
#include "PokemonLA_RegionNavigation.h"
#include "PokemonLA_TreeActions.h"
#include "PokemonLA/Resources/PokemonLA_PokemonInfo.h"
#include "PokemonLA/Inference/PokemonLA_StatusInfoScreenDetector.h"
#include "Pokemon/Inference/Pokemon_NameReader.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{


//void setup(ProgramEnvironment& env, ConsoleHandle& console, BotBaseContext& context){
//    goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Heights);
//    pbf_move_left_joystick(context, 170, 255, 30, 30);
//    change_mount(console, context, MountState::BRAVIARY_ON);
//    pbf_press_button(context, BUTTON_B, (6.35 * TICKS_PER_SECOND), 20);
//    pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
//    pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
//    pbf_press_button(context, BUTTON_PLUS, 20, (1.5 * TICKS_PER_SECOND));
//    pbf_move_left_joystick(context, 255, 127, 30, (0.5 * TICKS_PER_SECOND));
//    pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
//    pbf_move_right_joystick(context, 127, 255, (0.10 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
//    context.wait_for_all_requests();
//}

void route(ProgramEnvironment& env, ConsoleHandle& console, BotBaseContext& context, size_t pokemon){
    switch (pokemon) {
    case 0: //Aipom
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Coastlands_Beachside);
        pbf_move_left_joystick(context, 240, 255, 30, 30);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (21 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 255, 160, 30, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 127, 0, (3 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        break;
    case 1: //Burmy
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Heights);
        pbf_move_left_joystick(context, 170, 255, 30, 30);
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (6.35 * TICKS_PER_SECOND), 20);
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (1.5 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 255, 127, 30, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.10 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 2: //Cherrim
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Highlands_Mountain);
        pbf_move_left_joystick(context, 255, 35, 30, 30);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (3.35 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (1.5 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 0, 127, 20, 20);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.20 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 3: //Cherubi
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Arena);
        pbf_move_left_joystick(context, 255, 152, 30, 30);
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (25.5 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        break;
    case 4: //Combee
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Arena);
        pbf_move_left_joystick(context, 57, 255, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (2.75 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        break;
    case 5: //Heracross
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Highlands_Mountain);
        pbf_move_left_joystick(context, 0, 220, 30, 30);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 127, 0, (4 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.17 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        break;
    case 6: //Pachirisu
    case 7: //Vespiquen
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Mirelands_Bogbound);
        pbf_move_left_joystick(context, 0, 170, 30, 30);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (10 * TICKS_PER_SECOND), 20);
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        break;
    case 8: //Wormadam
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Fieldlands);
        pbf_move_left_joystick(context, 110, 255, 30, 30);
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (6.25 * TICKS_PER_SECOND), 20);
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 255, 100, 20, 20);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.2 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 9: //Geodude
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Fieldlands_Heights);
        pbf_move_left_joystick(context, 200, 255, 20, 20);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_press_button(context, BUTTON_B, (3.75 * TICKS_PER_SECOND), 20);
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (1 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 127, 255, 20, 20);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        break;
    case 10: //Graveler
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Highlands_Highlands);
        pbf_move_left_joystick(context, 255, 145, 20, 20);
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_left_joystick(context, 127, 0, (5 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 11: //Bonsly
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Mirelands_DiamondSettlement);
        pbf_move_left_joystick(context, 0, 150, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::BRAVIARY_ON);
        pbf_move_left_joystick(context, 127, 0, (10 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 12: //Bronzor
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Highlands_Mountain);
        pbf_move_left_joystick(context, 55, 255, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::WYRDEER_ON);
        pbf_press_button(context, BUTTON_B, (6.5 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.2 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 13: //Nosepass
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Highlands_Summit);
        pbf_move_left_joystick(context, 255, 175, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::WYRDEER_ON);
        pbf_press_button(context, BUTTON_B, (2.5 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.2 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    case 14: //Bergimite
        goto_camp_from_jubilife(env, console, context, TravelLocations::instance().Icelands_Snowfields);
        pbf_move_left_joystick(context, 115, 255, 20, (0.5 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_ZL, 20, (0.5 * TICKS_PER_SECOND));
        change_mount(console, context, MountState::WYRDEER_ON);
        pbf_press_button(context, BUTTON_B, (3.2 * TICKS_PER_SECOND), (1 * TICKS_PER_SECOND));
        pbf_press_button(context, BUTTON_PLUS, 20, (0.5 * TICKS_PER_SECOND));
        pbf_move_right_joystick(context, 127, 255, (0.2 * TICKS_PER_SECOND), (0.5 * TICKS_PER_SECOND));
        break;
    }
    context.wait_for_all_requests();
}

void return_to_jubilife(ProgramEnvironment& env, ConsoleHandle& console, BotBaseContext& context, size_t pokemon){
    goto_camp_from_overworld(env, console, context);
    switch (pokemon) {
    case 0:
        goto_professor(console, context, Camp::COASTLANDS_BEACHSIDE);
        break;
    case 1:
        goto_professor(console, context, Camp::FIELDLANDS_FIELDLANDS);
        break;
    case 2: //Cherrim
        goto_professor(console, context, Camp::HIGHLANDS_HIGHLANDS);
        break;
    case 3: //Cherubi
        goto_professor(console, context, Camp::FIELDLANDS_FIELDLANDS);
        break;
    case 4: //Combee
        goto_professor(console, context, Camp::FIELDLANDS_FIELDLANDS);
        break;
    case 5:
        goto_professor(console, context, Camp::HIGHLANDS_HIGHLANDS);
        break;
    case 6: //Pachirisu
    case 7: //Vespiquen
        goto_professor(console, context, Camp::MIRELANDS_MIRELANDS);
        break;
    case 8: //Wormadam
        goto_professor(console, context, Camp::FIELDLANDS_FIELDLANDS);
        break;
    case 9: //Geodude
        goto_professor(console, context, Camp::FIELDLANDS_FIELDLANDS);
        break;
    case 10: //Graveler
        goto_professor(console, context, Camp::HIGHLANDS_HIGHLANDS);
        break;
    case 11: //Bonsly
        goto_professor(console, context, Camp::MIRELANDS_MIRELANDS);
        break;
    case 12: //Bronzor
        goto_professor(console, context, Camp::HIGHLANDS_HIGHLANDS);
        break;
    case 13: //Nosepass
        goto_professor(console, context, Camp::HIGHLANDS_HIGHLANDS);
        break;
    case 14: //Bergimite
        goto_professor(console, context, Camp::ICELANDS_SNOWFIELDS);
        break;
    }
    from_professor_return_to_jubilife(env, console, context);
    context.wait_for_all_requests();
}

bool check_tree_for_battle(ConsoleHandle& console, BotBaseContext& context){

    pbf_press_button(context, BUTTON_ZR, (0.5 * TICKS_PER_SECOND), 20); //throw pokemon
    pbf_wait(context, (4.5 * TICKS_PER_SECOND));

    context.wait_for_all_requests();

    MountDetector mount_detector;
    MountState mount = mount_detector.detect(console.video().snapshot());

    if (mount != MountState::NOTHING){
       console.log("Battle not found. Tree might be empty.");
       return false;
    }

    console.log("Battle found!");

    BattleMenuDetector battle_menu_detector(console, console, true);
    wait_until(
       console, context, std::chrono::seconds(10),
       {
           {battle_menu_detector}
       }
    );

    return true;
}

void exit_battle(BotBaseContext& context){
    pbf_press_button(context, BUTTON_B, 20, 100);
    pbf_wait(context, (1 * TICKS_PER_SECOND));
    pbf_press_button(context, BUTTON_B, 20, 100);
    pbf_wait(context, (1 * TICKS_PER_SECOND));
    pbf_press_button(context, BUTTON_A, 20, 100);
    pbf_wait(context, (3 * TICKS_PER_SECOND));
    context.wait_for_all_requests();
}

PokemonDetails get_pokemon_details(ConsoleHandle& console, BotBaseContext& context, Language language){
    //Open Info Screen
    pbf_wait(context, (1 * TICKS_PER_SECOND));
    pbf_press_button(context, BUTTON_PLUS, 20, 20);
    pbf_wait(context, (1 * TICKS_PER_SECOND));
    pbf_press_button(context, BUTTON_R, 20, 20);
    pbf_wait(context, (1 * TICKS_PER_SECOND));

    context.wait_for_all_requests();

    QImage infoScreen = console.video().snapshot();

    StatusInfoScreenDetector detector;

    detector.process_frame(infoScreen, std::chrono::system_clock::now());
    detector.get_pokemon_name(console, infoScreen, language);

    return detector.details();;
}





















}
}
}
