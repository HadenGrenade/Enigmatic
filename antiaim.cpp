#include "aimbot.h"
#include "misc.h"

#include "src/semicore/globals.h"
#include "src/semicore/interfaces.h"
#include "src/valve/cusercmd.h"

/*
void hacks::antiaimrun(CUserCmd* cmd) {
	if (!v::aim.fov) return;
	if (!globals::localPlayer) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (cmd->buttons & CUserCmd::IN_USE) return;
	if (cmd->buttons & CUserCmd::IN_ATTACK) return;
	if (globals::localPlayer->IsDefusing())
		return;

	// not sure how to properly add a send packet function, so... 

}
*/