#include "misc.h"
#include "Vector3d.h"
#include "autowall.h"
#include "src/semicore/globals.h"
#include "src/semicore/interfaces.h"
#include "src/valve/cusercmd.h"
#include "src/semicore\globals.h"
#include "studio.h"
#define TIME_TO_TICKS( t )		( (int32_t)( 0.5f + (float)( t ) / TICK_INTERVAL ) )


// Checks if we can fire, used in other places
bool hacks::can_fire(CEntity* target) {
	CEntity* active_weapon = target->GetActiveWeapon();
	if (!active_weapon) return false;

	if (active_weapon->is_non_aim()) return false;			// Bad weapon or no ammo so don't aimbot
	if (target->next_attack() > interfaces::globals->currentTime) return false;
	if (active_weapon->next_primary_attack() > interfaces::globals->currentTime) return false;

	return true;
}

// Checks if the current weapon can shoot and all that
bool hacks::aimbot_weapon_check(bool check_scope) {
	if (globals::localPlayer->IsDefusing()) return false;

	CEntity* active_weapon = globals::localPlayer->GetActiveWeapon();
	if (!active_weapon) return false;

	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return false;

	switch (weapon_data->weapon_type) {									// Only aimbot on weapons that shoot
	case CEntity::EWeaponType::WEAPONTYPE_MACHINEGUN:
	case CEntity::EWeaponType::WEAPONTYPE_RIFLE:
	case CEntity::EWeaponType::WEAPONTYPE_SUBMACHINEGUN:
	case CEntity::EWeaponType::WEAPONTYPE_SHOTGUN:
	case CEntity::EWeaponType::WEAPONTYPE_SNIPER:
	case CEntity::EWeaponType::WEAPONTYPE_PISTOL: {
		if (!can_fire(globals::localPlayer)) return false;					// Check if we can fire

		if (check_scope) {
			if (weapon_data->weapon_type == CEntity::EWeaponType::WEAPONTYPE_SNIPER
				&& !globals::localPlayer->IsScoped()
				&& !v::aim.aimbot_noscope) return false;		// We are not scoped and have the noscope option disabled
		}

		break;
	}
	default: return false;
	}

	// (We reached here without return so we are good to use aimbot)
	return true;
}

vec3_t get_best_target(CUserCmd* cmd, CEntity* active_weapon) {
	vec3_t final_target(0, 0, 0);					// Position of best hitbox. Will be returned
	// Check if we have weapon data before doing anything else
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return final_target;

	// Get eye pos from localplayer
	auto local_eye_pos = globals::localPlayer->eye_angles();

	// Players will get appended here with their fov. Then the vector will get ordered.
	std::vector<std::pair<float, CEntity*>> target_list{};

	// Iterate players and store the fov to their eyes. It is just so we can have an aproximate priority by crosshair distance.
	for (int i = 1; i <= interfaces::globals->maxClients; i++) {
		auto entity = reinterpret_cast<CEntity*>(interfaces::entityList->GetClientEntity(i));
		if (!entity
			|| entity == globals::localPlayer
			|| !entity->IsAlive()
			|| entity->IsDormant()
			|| entity->HasGunGameImmunity()) continue;
		if (!hacks::is_enemy(entity) && !v::aim.target_friends) continue;

		// We get the eye position of the current entity so we can store it, and then order by it. This doesn't have to be precise
		auto entity_pos = entity->eye_angles();

		// Calculate relative angle to target
		vec3_t aim_angle = aim_angle.calculate_relative_angle(local_eye_pos, entity_pos, cmd->viewAngles);
		aim_angle.clamp();

		// Get the fov to target. Lower == closer to crosshair == better
		const float fov = std::hypot(aim_angle.x, aim_angle.y);

		// Push to target vector the entity with its fov
		target_list.push_back({ fov, entity });
	}

	// After storing the players, order them from lower fov to greater fov (priority)
	std::sort(target_list.begin(), target_list.end(), [](const std::pair<float, CEntity*>& a, const std::pair<float, CEntity*>& b) -> bool {
		return a.first < b.first;
		});

	// Store selected hitboxes
	std::vector<int> all_hitboxes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };	// For bodyaim if lethal
	std::vector<int> selected_hitboxes;
	if (v::aim.hitboxes == 1) {		// Head
		selected_hitboxes.emplace_back(hitbox_head);
	}
	if (v::aim.hitboxes == 2) {		// Neck
		selected_hitboxes.emplace_back(hitbox_neck);
	}
	if (v::aim.hitboxes == 2) {		// Chest
		selected_hitboxes.emplace_back(hitbox_pelvis);
		selected_hitboxes.emplace_back(hitbox_stomach);
		selected_hitboxes.emplace_back(hitbox_lower_chest);
		selected_hitboxes.emplace_back(hitbox_chest);
		selected_hitboxes.emplace_back(hitbox_upper_chest);
	}
	if (v::aim.hitboxes == 4) {		// Arms
		selected_hitboxes.emplace_back(hitbox_right_hand);
		selected_hitboxes.emplace_back(hitbox_left_hand);
		selected_hitboxes.emplace_back(hitbox_right_upper_arm);
		selected_hitboxes.emplace_back(hitbox_left_upper_arm);
		selected_hitboxes.emplace_back(hitbox_right_forearm);
		selected_hitboxes.emplace_back(hitbox_left_forearm);

	}
	if (v::aim.hitboxes == 6) {		// Legs
		selected_hitboxes.emplace_back(hitbox_right_thigh);
		selected_hitboxes.emplace_back(hitbox_left_thigh);
		selected_hitboxes.emplace_back(hitbox_right_calf);
		selected_hitboxes.emplace_back(hitbox_left_calf);
		selected_hitboxes.emplace_back(hitbox_right_foot);
		selected_hitboxes.emplace_back(hitbox_left_foot);

	}

	// Iterate the ordered target list
	for (const std::pair<float, CEntity*> item : target_list) {
		// Check if we are checking a target with more fov than needed
		if (item.first > v::aim.aimbot_fov) break;

		const auto entity = item.second;
		const float entity_health = entity->GetHealth();

		// Will store the best damage and hitbox of the current player
		float best_player_damage = 0.f;
		vec3_t best_player_hitbox(0, 0, 0);

		// Iterate all possible hitboxes, even if not enabled (for checking bodyaim if lethal)
		for (const int hitbox : all_hitboxes) {
			auto hitbox_pos = entity->get_hitbox_position_fixed(hitbox);
			bool enabled_hitbox = std::find(selected_hitboxes.begin(), selected_hitboxes.end(), hitbox) != selected_hitboxes.end();		// Enabled by user

			autowall_data_t autowall_data = { false, 0.f };

			if (v::aim.autowall < 2) {
				// If "Only visible" and we can't see it, ignore hitbox
				if (v::aim.autowall == 0 && !globals::localPlayer->can_see_player_pos(entity, hitbox_pos))
					continue;

				// Get autowall data and check if we can make enough damage or kill. autowall::handle_walls() takes care of stuff like "bodyaim if lethal" and "only visible",
				// so it will only return damage for valid hitboxes
				autowall_data = hacks::handle_walls(globals::localPlayer, entity, hitbox_pos, weapon_data, enabled_hitbox);

				// Check if the autowall data was invalid
				if (autowall_data.damage < 0.f)
					continue;

				// Check if the returned damage is enough or if we can kill the target (we dont need to worry about bodyaim_if_lethal here)
				if (autowall_data.damage < (int)v::aim.min_damage && !autowall_data.lethal)
					continue;
			}
			else if (v::aim.autowall == 2) {
				// @todo: bodyaim_if_lethal would not work with "ignore walls" because we dont run autowall
				// We are trying to use ignore walls with disabled hitbox.
				if (!enabled_hitbox) continue;

				// We don't care about walls and we found a valid hitbox, return it
				return hitbox_pos;
			}

			// If we can kill, we don't care about any other players, since we are checking by fov priority
			if (autowall_data.lethal || autowall_data.damage >= entity_health)
				return hitbox_pos;

			// Check what the best hitbox would be based on damage, then save it as "this player's best hitbox"
			if (autowall_data.damage > best_player_damage) {
				best_player_damage = autowall_data.damage;
				best_player_hitbox = hitbox_pos;
			}
		}

		/*
		 * What we are doing here is saving the hitbox with the most damage from the current player (best_player_hitbox) as the final target ONLY if we don't have a final target yet (we found a closer non-lethal target).
		 * We do it this way so:
		 *		1. We check if we can do damage to the closest player
		 *		2. If we can't (after checking hitboxes, all are 0 damage) go to the next player until we find a target that we can damage inside our fov (fov is checked on the first line of the loop)
		 *		3. Once we find a valid hitbox (max damage of closest player), store it as the final target, but don't return/break inmediately
		 *		4. If we have priorize_lethal_targets, keep iterating the rest of the players in the vector to see if we can deal lethal damage to a hitbox, then return that instead (just when we find the lethal hitbox)
		 */
		if (best_player_damage > 0.f && final_target.is_zero()) {
			final_target = best_player_hitbox;

			if (!v::aim.priorize_lethal_targets) break;
		}
	}

	return final_target;		// vec3_t position of the best bone/hitbox. Zero if not found.
}

void hacks::run_Ragebot(CUserCmd* cmd) {
	if /* (!*/(!v::aim.autofire)
	//	&& !(!v::aim.autofire && (cmd->buttons & cmd::in_attack))) return;			
	if (!v::aim.ragebot) return;
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame()) return;
	if (!globals::localPlayer) return;
	if (!aimbot_weapon_check(true)) return;

	// We need to get weapon_type here too for aim_punch and for autowall
	CEntity* active_weapon = globals::localPlayer->GetActiveWeapon();
	if (!active_weapon) return;
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return;

	auto local_eye_pos = globals::localPlayer->eye_angles();		// Get eye pos from origin player_t
	vec3_t target = get_best_target(cmd, active_weapon);
	if (target.is_zero()) return;

	vec3_t aim_angle = aim_angle.calculate_angle(local_eye_pos, target);
	aim_angle.clamp();

	vec3_t local_aim_punch{};	// Initialize at 0 because we only want aim punch with rifles
	if (v::aim.non_rifle_aimpunch) {
		local_aim_punch = globals::localPlayer->get_aim_punch();
	}
	else {
		switch (weapon_data->weapon_type) {
		case CEntity::EWeaponType::WEAPONTYPE_RIFLE:
		case CEntity::EWeaponType::WEAPONTYPE_SUBMACHINEGUN:
		case CEntity::EWeaponType::WEAPONTYPE_MACHINEGUN:
			local_aim_punch = globals::localPlayer->get_aim_punch();
		}
	}

	vec3_t enemy_angle = (aim_angle - local_aim_punch) - cmd->viewAngles;
	enemy_angle.clamp();

	vec3_t angle_diff = enemy_angle;
	//THIS IS VERY WELL DONE AND IS SIGNIFICANTLY BETTER THAN OUR OLD CODE, AS WE DIDNT EVEN DO THIS... SO I MIGHT JUST DITCH OUR OLD AIMBOT BUT IM NOT SURE, I THINK THIS IS GONNA BE CALLED A SEMI RAGE TBH.
	if (!v::aim.silent) angle_diff *= (1.f - v::aim.aimbot_smoothing);	// Scale acording to smoothing if not silent

	vec3_t final_angle = cmd->viewAngles + angle_diff;		// The current angle before the aimbot + what we should move
	if (!v::aim.silent)
		interfaces::engine->SetViewAngles(final_angle);

	cmd->viewAngles = final_angle;

	if (v::aim.autofire)
		cmd->buttons |= CUserCmd::IN_ATTACK;
}
