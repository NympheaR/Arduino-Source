/*  Egg Routines
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *      This file requires (PABB_PABOTBASE_LEVEL >= 31).
 *
 */

#ifndef PokemonAutomation_PokemonSwSh_Commands_EggRoutines_H
#define PokemonAutomation_PokemonSwSh_Commands_EggRoutines_H

#include "ClientSource/Connection/BotBase.h"

namespace PokemonAutomation{
namespace NintendoSwitch{


void eggfetcher_loop        (BotBaseContext& context);
void move_while_mashing_B   (BotBaseContext& context, uint16_t duration);
void spin_and_mash_A        (BotBaseContext& context, uint16_t duration);
void travel_to_spin_location(BotBaseContext& context);
void travel_back_to_lady    (BotBaseContext& context);




}
}
#endif
