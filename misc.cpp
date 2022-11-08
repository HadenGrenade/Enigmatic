#include "misc.h"

// access global variables

// access interfaces
#include "src/semicore/interfaces.h"
#include "src/semicore/globals.h"
void hacks::RunBunnyHop(CUserCmd* cmd) noexcept
{
	if (!v::misc.bunnyHop)
		return;

		if (!(globals::localPlayer->GetFlags() & 1))
			cmd->buttons &= ~CUserCmd::IN_JUMP;
	
	
}
 