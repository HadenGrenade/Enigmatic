#include "aimbot.h"
#include "misc.h"
#include "istudiorender.h"
#include "src/semicore/globals.h"
#include "src/semicore/ainterfaces.h"
#include "src/valve/cusercmd.h"
#include "studio.h"
#include "autowall.h"
#include "ienginetrace.h"
#include "Vector3d.h"

#pragma region MULTIPLIERS
float hacks::get_damage_multiplier(int hit_group, float hs_multiplier) {
	switch (hit_group) {
	case hitgroup_head:			return hs_multiplier;		// Changes depending on weapon
	case hitgroup_stomach:		return 1.25f;
	case hitgroup_leftleg:
	case hitgroup_rightleg:		return 0.75f;
	default:					return 1.0f;
	}
}

bool hacks::is_armored(int hit_group, bool helmet) {
	switch (hit_group) {
	case hitgroup_head:			return helmet;
	case hitgroup_chest:
	case hitgroup_stomach:
	case hitgroup_leftarm:
	case hitgroup_rightarm:		return true;
	default:					return false;
	}
}

#pragma endregion

#pragma region BULLET PENETRATION
bool hacks::trace_to_exit(CTrace& enter_trace, vec3_t& start, const vec3_t& direction, vec3_t& end, CTrace& exit_trace) {
	static std::uintptr_t trace_to_exit = reinterpret_cast<std::uintptr_t>(memory::PatternScan("client.dll", sig_trace_to_exit));
	if (!trace_to_exit) return false;
	const auto trace_to_exit_fn = trace_to_exit;
	if (!trace_to_exit_fn) return false;

	bool result = false;
	_asm {
		push 0
		push 0
		push 0
		push exit_trace
		mov eax, direction
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov eax, start
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov edx, enter_trace
		mov ecx, end
		call trace_to_exit_fn
		add esp, 40
		mov result, al
	}
	return result;
}

static bool hacks::handle_bullet_penetration(surface_data* enter_surface_data, CTrace& enter_trace, const vec3_t& direction, vec3_t& start, float penetration, float& damage) {
	CTrace exit_trace;
	vec3_t dummy;

	if (!trace_to_exit(enter_trace, enter_trace.endPos, direction, dummy, exit_trace))
		return -1.0f;

	auto exit_surface_data = interfaces::surface_props_physics->get_surface_data(exit_trace.surface.surfaceProps);

	float damage_modifier = 0.16f;
	float penetration_modifier = (enter_surface_data->penetrationmodifier + exit_surface_data->penetrationmodifier) / 2.0f;

	if (enter_surface_data->material == 71 || enter_surface_data->material == 89) {
		damage_modifier = 0.05f;
		penetration_modifier = 3.0f;
	}
	else if (enter_trace.contents >> 3 & 1 || enter_trace.surface.flags >> 7 & 1) {
		penetration_modifier = 1.0f;
	}

	if (enter_surface_data->material == exit_surface_data->material) {
		if (exit_surface_data->material == 85 || exit_surface_data->material == 87)
			penetration_modifier = 3.0f;
		else if (exit_surface_data->material == 76)
			penetration_modifier = 2.0f;
	}

	damage -= 11.25f / penetration / penetration_modifier + damage * damage_modifier + (exit_trace.endPos - enter_trace.endPos).length_sqr() / 24.0f / penetration_modifier;


	start = exit_trace.endPos;		// Set start of next trace to the current end

	if (damage < 1.0f) return false;
	return true;
}
#pragma endregion

#pragma region AUTOWALL
// Used to check if target it visible or hittable. Used in aimbot.
// enabled_hitbox will be used to know what hitboxes are enabled by the user (cuz now its iterating all due to bodyaim_if_lethal)
autowall_data_t hacks::handle_walls(CEntity* local_player, CEntity* entity, const vec3_t& destination, const weapon_info_t* weapon_data, bool enabled_hitbox) {
	if (!v::aim.bodyaim_if_lethal && !enabled_hitbox) return { false, -1.f };

	float damage = static_cast<float>(weapon_data->weapon_damage);
	vec3_t start = local_player->eye_angles();
	vec3_t direction = (destination - start);
	direction /= direction.length();
	float distance = 0.f;			// Will store traveled distance in the loop, max is the weapon range

	CRay ray;						// Declare ray once before loop
	CTraceFilter filter;
	filter.skipEntity = local_player;		// Initialize filter for ray before loop

	int hits_left = 4;
	while (damage >= 1.0f && hits_left) {
		vec3_t end = start + (direction * (weapon_data->weapon_range - distance));
		ray.initialize(start, end);

		static CTrace trace;

		interfaces::engineTrace->TraceRay(ray, 0x4600400B, filter, trace);
		if (trace.fraction == 1.0f) break;

		distance += trace.fraction * (weapon_data->weapon_range - distance);
		damage = damage * get_damage_multiplier(trace.hitgroup, weapon_data->weapon_headshot_multiplier) * powf(weapon_data->weapon_range_mod, distance / 500.0f);
		if (trace.entity == entity && trace.hitgroup > hitgroup_generic && trace.hitgroup <= hitgroup_rightleg) {
			if (float armor_ratio{ weapon_data->weapon_armor_ratio / 2.0f }; is_armored(trace.hitgroup, trace.entity->has_helmet()))
				damage -= (trace.entity->armor() < damage * armor_ratio / 2.0f ? trace.entity->armor() * 4.0f : damage) * (1.0f - armor_ratio);

			// If we can kill and we have the setting enabled, ignore enabled hitboxes and shoot
			if (v::aim.bodyaim_if_lethal && reinterpret_cast<CEntity*>(entity)->GetHealth() < damage)
				return { true, damage };
			// If we can't kill, the best place to shoot is the closest enabled hitbox
			else if (enabled_hitbox)
				return { false, damage };
		}
		// Return invalid if we care only about visible and we dont see the target. This should never happen.
		//if (variables::aim::autowall.idx == 0) return { false, -1.f };

		const auto surface_data = interfaces::surface_props_physics->get_surface_data(trace.surface.surfaceProps);
		if (surface_data->penetrationmodifier < 0.1f) break;

		// Start and damage are changed from handle_bullet_penetration()
		if (!hacks::handle_bullet_penetration(surface_data, trace, direction, start, weapon_data->weapon_penetration, damage))
			return { false, -1.f };

		hits_left--;
	}

	return { false, -1.f };
}
#pragma endregion
