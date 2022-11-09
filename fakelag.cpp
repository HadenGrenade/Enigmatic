#include "aimbot.h"
#include "misc.h"

#include "src/semicore/globals.h"
#include "src/semicore/interfaces.h"
#include "src/valve/cusercmd.h"
#include "Vector3d.hpp"

//ok so i made the actual code but not the hook cuz I couldn't figure out how you setup hooks
//you basically just have to update net latency in frameStageNotify hook, simple enough, you'll figure it out :)

void hacks::run_fl(bool& send_packet) noexcept 
{
    if (!globals::localPlayer || !globals::localPlayer->IsAlive())
        return;

    const auto netChannel = interfaces::engine->get_net_channel_info();
    if (!netChannel)
        return;

    auto chokedPackets = 0; //temp i fix later
    if (v::antiaim.fakelag)
    {
        chokedPackets = v::antiaim.limit;
			/* dis is just random fl, it works but cba to make a dropdown to choose
            srand(static_cast<unsigned int>(time(NULL))); 
            chokedPackets = rand() % config->fakelag.limit + 1;
            */
    }

    //very gehto, make a get get maxUserCmdProcessTicks function later
    chokedPackets = std::clamp(chokedPackets, 0, 16);

    send_packet = chokedPackets;
}

