/*  Tera Roller
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_TeraRoller_H
#define PokemonAutomation_PokemonSV_TeraRoller_H

#include "Common/Cpp/Options/BooleanCheckBoxOption.h"
#include "Common/Cpp/Options/SimpleIntegerOption.h"
#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "NintendoSwitch/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
   struct VideoSnapshot;
namespace NintendoSwitch{
namespace PokemonSV{

class TeraRoller;


class TeraRoller_Descriptor : public SingleSwitchProgramDescriptor{
public:
    TeraRoller_Descriptor();

    struct Stats;
    virtual std::unique_ptr<StatsTracker> make_stats() const override;

};




class TeraRollerOpponentFilter : public GroupOption{
public:
    TeraRollerOpponentFilter();

    bool should_battle(size_t stars, const std::string& pokemon) const;

    SimpleIntegerOption<uint8_t> MIN_STARS;
    SimpleIntegerOption<uint8_t> MAX_STARS;

};



class TeraRoller : public SingleSwitchProgramInstance{
public:
    TeraRoller();
    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    BooleanCheckBoxOption CHECK_ONLY_FIRST;
    TeraRollerOpponentFilter FILTER;

    enum class EventCheckMode{
        CHECK_ALL,
        CHECK_ONLY_EVENT,
        CHECK_ONLY_NONEVENT,
    };
    EnumDropdownOption<EventCheckMode> EVENT_CHECK_MODE;

    //  Notifications
    EventNotificationOption NOTIFICATION_STATUS_UPDATE;
    EventNotificationOption NOTIFICATION_SHINY;
    EventNotificationOption m_notification_noop;
    EventNotificationsOption NOTIFICATIONS;

};




}
}
}
#endif
