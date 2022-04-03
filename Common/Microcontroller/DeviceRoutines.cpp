/*  Device Routines
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "DeviceRoutines.h"

namespace PokemonAutomation{
namespace Microcontroller{


uint32_t protocol_version(BotBase& botbase){
    pabb_MsgAckRequestI32 response;
    botbase.issue_request_and_wait(
        DeviceRequest_protocol_version()
    ).convert<PABB_MSG_ACK_REQUEST_I32>(botbase.logger(), response);
    return response.data;
}
uint32_t program_version(BotBase& botbase){
    pabb_MsgAckRequestI32 response;
    botbase.issue_request_and_wait(
        DeviceRequest_program_version()
    ).convert<PABB_MSG_ACK_REQUEST_I32>(botbase.logger(), response);
    return response.data;
}
uint8_t program_id(BotBase& botbase){
    pabb_MsgAckRequestI8 response;
    botbase.issue_request_and_wait(
        DeviceRequest_program_id()
    ).convert<PABB_MSG_ACK_REQUEST_I8>(botbase.logger(), response);
    return response.data;
}


}
}
