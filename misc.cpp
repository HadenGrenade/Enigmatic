#include "misc.h"

// access global variables

// access interfaces
#include "src/semicore/interfaces.h"
#include "src/semicore/globals.h"
void hacks::RunBunnyHop(CUserCmd* cmd) noexcept
{
	if (!v::misc.bunnyHop)
		return;
	//CEntity::FL_ONGROUND
		if (!(globals::localPlayer->GetFlags() & 1))
			cmd->buttons &= ~CUserCmd::IN_JUMP;
	
	
}
// ripped this bitch from uc  --https://www.unknowncheats.me/forum/counterstrike-global-offensive/144026-basic-auto-strafe.html
//it doesnt work, no clue why, not fixing it, feel free to pysik! 
/*
void hacks::autostrafe(CUserCmd* cmd) noexcept
{
	if (!v::misc.autoStrafe)
	return;
		if (!(globals::localPlayer->GetFlags() & 1)) {
			if (cmd->mouseDeltaX < 0)
				cmd->sideMove = -450.0f;
			if (cmd->mouseDeltaY > 0)
				cmd->sideMove = 450.0f;
		}
	
 }*/