#include "src/valve/centity.h"
#include "src/semicore/ainterfaces.h"
#include "misc.h"


bool CEntity::can_see_player_pos(CEntity* player, const vec3_t& pos) {
	CTraceFilter filter;					// trace_filter derived from i_trace_filter
	filter.skipEntity = this;						// Add the origin player_t to skip filter

	auto start = eye_angles();				// Get eye pos from origin player_t
	auto dir = (pos - start).normalized();	// idk

	CRay ray;
	ray.initialize(start, pos);				// Initialize ray like this

	CTrace tr;
	interfaces::engineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, filter, tr);

	return tr.entity == player || tr.fraction > 0.97f;	// We found the entity and the fraction is long enough to not collide with shit
}

vec3_t CEntity::get_hitbox_position_fixed(int hitbox_id) {
	matrix_t bone_matrix[MAXSTUDIOBONES];
	vec3_t2 urmother;

	if (setup_bones_fixed(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = interfaces::modelInfo->get_studio_model(model());

		if (studio_model) {
			auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

			if (hitbox) {
				auto min = vec3_t{}, max = vec3_t{};
				urmother.transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
				urmother.transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

				return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}
		}
	}
	return vec3_t{};
}

bool CEntity::setup_bones_fixed(matrix_t* out, int max_bones, int mask, float time) {
	if (!this) return false;
	using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);

	// Fix bone matrix. First backup render and abs_origina
	int* render = reinterpret_cast<int*>(this + 0x274);
	int render_backup = *render;

	vec3_t actual_abs_origin = abs_origin();

	*render = 0;

	using abs_fn = void(__thiscall*)(CEntity*, const vec3_t&);
	static abs_fn set_abs_origin = hacks::relative_to_absolute<abs_fn>(memory::PatternScan("client.dll", "E8 ? ? ? ? EB 19 8B 07") + 1);

	set_abs_origin(this, origin());

	auto result = (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);		// Get original result from vfunc with origin

	// Restore old abs_origin and render
	set_abs_origin(this, actual_abs_origin);
	*render = render_backup;

	return result;
}

bool CEntity::is_knife() {
	switch (this->item_definition_index()) {
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFEGG:
	case WEAPON_BAYONET:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_CSS:
		return true;
	default:
		return false;
	}
}

bool  CEntity::is_grenade() {
	switch (this->item_definition_index()) {
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_DECOY:
		return true;
	default:
		return false;
	}
}

weapon_info_t* CEntity::get_weapon_data() {
	return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
}