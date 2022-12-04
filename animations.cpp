#include "misc.h"

#include "src/semicore/globals.h"
#include "src/semicore/ainterfaces.h"
#include "src/valve/cusercmd.h"
//#include "vector3d.h"




// WHOLE THIGN DONT WORK CUZ NEVER CALLED AND THAT MENAS IT GIVES LINKER IF I DONT COMMENT SO
/*

// Used in frame_stage_notify - FRAME_RENDER_START
void hacks::run_local_animations() {
	if (!globals::localPlayer || !globals::localPlayer->IsAlive()) return;
	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsInGame()) return;
	if (!interfaces::input->camera_in_third_person) return;		// For now we only care about local animations when on thirdperson

	// Save the angles
	auto angles = globals::localPlayer->eye_angles();
	angles.z = 0.f;

	// Set them and update to apply
	interfaces::prediction->set_local_view_angles(angles);

	/*
	 * @todo: Fix crash when running full_update()
	if (globals::forcing_update)
		csgo::local_player->update_client_side_animations();
	
}*/