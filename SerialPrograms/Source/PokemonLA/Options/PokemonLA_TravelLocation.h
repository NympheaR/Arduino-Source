/*  Travel Location
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_TravelLocation_H
#define PokemonAutomation_PokemonLA_TravelLocation_H

#include "Common/Cpp/Options/EnumDropdownOption.h"
#include "PokemonLA/PokemonLA_TravelLocations.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{

class TravelLocationOption : public DropdownOption{
public:
    TravelLocationOption();
    operator TravelLocation() const;
};





}
}
}
#endif
