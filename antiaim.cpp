#include "aimbot.h"
#include "misc.h"

#include "src/semicore/globals.h"
#include "src/semicore/interfaces.h"
#include "src/valve/cusercmd.h"
#include "Vector3d.hpp"



// Used in create_move hook. Inside prediction
void hacks::runAntiaim(CUserCmd * cmd, bool& send_packet) {
	if (!v::antiaim.antiaim) return;
	if (!globals::localPlayer || !globals::localPlayer->IsAlive()) return;
	const int move_type = CEntity::move_type();

	if (move_type == CEntity::movetype_ladder || move_type == CEntity::movetype_noclip || move_type == CEntity::movetype_observer) return;     // Ladder or noclip

	CEntity* active_weapon = globals::localPlayer->GetActiveWeapon();

	// Pitch
	cmd->viewAngles.x = v::antiaim.pitch;

	// Use different var for yaw in case we are spinning or with peek aa
	static bool peek_right = true;
	constexpr float right_peek_yaw = 140.f;
	constexpr float left_peek_yaw = -140.f;
	static float yaw = 0.f;

	if (v::antiaim.spinbot) {
		yaw += v::antiaim.spinbot_speed / 2;       // Half speed for more control
	}
	else if (v::antiaim.peek_aa) {
		static bool was_pressed = false;
		if (v::antiaim.temp) {
			if (!was_pressed) peek_right = !peek_right;
			was_pressed = true;
		}
		else {
			was_pressed = false;
		}

		// Change yaw to peek dir
		yaw = peek_right ? right_peek_yaw : left_peek_yaw;
	}
	else {
		yaw = v::antiaim.yaw;
	}

	// Yaw
	if (!send_packet) yaw += v::antiaim.desync;		// Add desync
	cmd->viewAngles.y -= yaw;			// Real

	cmd->viewAngles.clamp();


	//micromovemtns 
	if (fabsf(cmd->sideMove) < 5.0f) {
		// If we are ducking make the movement bigger
		if (cmd->buttons & FL_DUCKING)
			cmd->sideMove = cmd->tickCount & 1 ? 3.25f : -3.25f;
		// Else just make it normal
		else
			cmd->sideMove = cmd->tickCount & 1 ? 1.1f : -1.1f;
	}

	// @todo: lby and fakelag
	// @note: https://github.com/LWSS/Fuzion/blob/0a4d775e17aba7a723aadce5b80898705e0bd6ff/src/Hacks/antiaim.cpp#L240
	// @note: https://github.com/LWSS/Fuzion/blob/master/src/Hacks/fakelag.cpp
}