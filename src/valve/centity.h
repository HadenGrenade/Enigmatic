#pragma once
#include "../util/memory.h"
#include "../../netvars.h"
#include "../../models.h"
#include "../../cclientclass.h"
#include "cvector.h"
#include "../../cmatrix.h"
#include "../../studio.h" 
#include "../../ienginetrace.h"
#include "../../misc.h"
#include "../semicore/interfaces.h"

class CSPlayer;
class CEntity;
class IClientUnknown
{
public:
	constexpr CEntity* GetBaseEntity() noexcept
	{
		return memory::Call<CEntity*>(this, 7);
	}
};

class IClientRenderable
{
public:
	constexpr IClientUnknown* GetIClientUnknown() noexcept
	{
		return memory::Call<IClientUnknown*>(this, 0);
	}
};

class CModel;
class CEntity
{
public:
	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	enum EFlags : std::int32_t
	{
		FL_ONGROUND = (1 << 0),
		FL_DUCKING = (1 << 1),
		FL_ANIMDUCKING = (1 << 2),
		FL_WATERJUMP = (1 << 3),
		FL_ONTRAIN = (1 << 4),
		FL_INRAIN = (1 << 5),
		FL_FROZEN = (1 << 6),
		FL_ATCONTROLS = (1 << 7),
		FL_CLIENT = (1 << 8),
		FL_FAKECLIENT = (1 << 9),
		FL_INWATER = (1 << 10),
		FL_FLY = (1 << 11),
		FL_SWIM = (1 << 12),
		FL_CONVEYOR = (1 << 13),
		FL_NPC = (1 << 14),
		FL_GODMODE = (1 << 15),
		FL_NOTARGET = (1 << 16),
		FL_AIMTARGET = (1 << 17),
		FL_PARTIALGROUND = (1 << 18),
		FL_STATICPROP = (1 << 19),
		FL_GRAPHED = (1 << 20),
		FL_GRENADE = (1 << 21),
		FL_STEPMOVEMENT = (1 << 22),
		FL_DONTTOUCH = (1 << 23),
		FL_BASEVELOCITY = (1 << 24),
		FL_WORLDBRUSH = (1 << 25),
		FL_OBJECT = (1 << 26),
		FL_KILLME = (1 << 27),
		FL_ONFIRE = (1 << 28),
		FL_DISSOLVING = (1 << 29),
		FL_TRANSRAGDOLL = (1 << 30),
		FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
	};
	enum move_type {
		movetype_none = 0,
		movetype_isometric,
		movetype_walk,
		movetype_step,
		movetype_fly,
		movetype_flygravity,
		movetype_vphysics,
		movetype_push,
		movetype_noclip,
		movetype_ladder,
		movetype_observer,
		movetype_custom,
		movetype_last = movetype_custom,
		movetype_max_bits = 4,
		max_movetype
	};
	enum item_definition_indexes {
		WEAPON_NONE = 0,
		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALILAR = 13,
		WEAPON_M249,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_TASER,
		WEAPON_HKP2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG556,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1_SILENCER,
		WEAPON_USP_SILENCER,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_BAYONET = 500,
		WEAPON_KNIFE_CSS = 503,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_PARACORD,
		WEAPON_KNIFE_SURVIVAL,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_NOMAD,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		WEAPON_KNIFE_SKELETON = 525,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};
	enum EWeaponType : int
	{
		WEAPONTYPE_KNIFE = 0,
		WEAPONTYPE_PISTOL = 1,
		WEAPONTYPE_SUBMACHINEGUN = 2,
		WEAPONTYPE_RIFLE = 3,
		WEAPONTYPE_SHOTGUN = 4,
		WEAPONTYPE_SNIPER = 5,
		WEAPONTYPE_MACHINEGUN = 6,
		WEAPONTYPE_C4 = 7,
		WEAPONTYPE_PLACEHOLDER = 8,
		WEAPONTYPE_GRENADE = 9,
		WEAPONTYPE_HEALTHSHOT = 11,
		WEAPONTYPE_FISTS = 12,
		WEAPONTYPE_BREACHCHARGE = 13,
		WEAPONTYPE_BUMPMINE = 14,
		WEAPONTYPE_TABLET = 15,
		WEAPONTYPE_MELEE = 16
	};

public: // netvars
	NETVAR(eye_angles, "DT_CSPlayer->m_angEyeAngles", vec3_t)
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", std::int32_t);
	NETVAR(IsScoped, "CCSPlayer->m_bIsScoped", bool)
	NETVAR(IsDefusing, "CCSPlayer->m_bIsDefusing", bool)
	NETVAR(HasGunGameImmunity, "CCSPlayer->m_bGunGameImmunity", bool)
	NETVAR(velocity, "CBasePlayer->m_vecVelocity[0]", Vector)
	NETVAR(GetClip, "CBaseCombatWeapon->m_iClip1", int)
	NETVAR(get_tick_base, "DT_CSPlayer->m_nTickBase", int)
	NETVAR(fall_velocity, "DT_BasePlayer->m_flFallVelocity", float)
	NETVAR(shots_fired, "DT_CSPlayer->m_iShotsFired", int)		
	NETVAR(has_helmet, "DT_CSPlayer->m_bHasHelmet", bool)
	NETVAR(armor, "DT_CSPlayer->m_ArmorValue", int)
	NETVAR(item_definition_index, "DT_BaseAttributableItem->m_iItemDefinitionIndex", short)
	NETVAR(next_attack, "DT_CSPlayer->m_flNextAttack", float)
		NETVAR(origin, "DT_BasePlayer->m_vecOrigin", vec3_t);

public: // renderable virtual functions (+0x4)
	constexpr CModel* GetModel() noexcept
	{
		return memory::Call<CModel*>(this + 0x4, 8);
	}

	constexpr bool SetupBones(CMatrix3x4* out, std::int32_t max, std::int32_t mask, float currentTime) noexcept
	{
		return memory::Call<bool>(this + 0x4, 13, out, max, mask, currentTime);
	}

public: // networkable virtual functions (+0x8)
	constexpr CClientClass* GetClientClass() noexcept
	{
		return memory::Call<CClientClass*>(this + 0x8, 2);
	}

	constexpr bool IsDormant() noexcept
	{
		return memory::Call<bool>(this + 0x8, 9);
	}
		vec3_t get_aim_punch() {
		vec3_t ret;
		using original_fn = void(__thiscall*)(void*, vec3_t*);
		(*(original_fn**)this)[346](this, &ret);
		return ret;
	}
		void set_view_angles(vec3_t& angles) {
			using original_fn = void(__thiscall*)(CEntity*, vec3_t&);
			return (*(original_fn**)this)[19](this, angles);
		}
	constexpr std::int32_t GetIndex() noexcept
	{
		return memory::Call<std::int32_t>(this + 0x8, 10);
	}
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
public: // entity virtual functions
	constexpr const CVector& GetAbsOrigin() noexcept
	{
		return memory::Call<const CVector&>(this, 10);
	}

	NETVAR(GetTeam, "CBaseEntity->m_iTeamNum", int)

	//	NETVAR(GetVelocity, "CBaseEntity->m_iTeamNum", int)

	

	constexpr std::int32_t GetHealth() noexcept
	{
		return memory::Call<std::int32_t>(this, 122);
	}

	constexpr bool IsAlive() noexcept
	{
		return memory::Call<bool>(this, 156);
	}
	

	constexpr bool IsPlayer() noexcept
	{
		return memory::Call<bool>(this, 158);
	}

	constexpr bool IsWeapon() noexcept
	{
		return memory::Call<bool>(this, 166);
	}

	constexpr CEntity* GetActiveWeapon() noexcept
	{
		return memory::Call<CEntity*>(this, 268);
	}
		
	bool setup_bones_fixed(matrix_t* out, int max_bones, int mask, float time) {
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

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}
	
	vec3_t get_hitbox_position_fixed(int hitbox_id) {
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

	bool is_knife() {
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

	bool is_grenade() {
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
	NETVAR(next_primary_attack, "DT_BaseCombatWeapon->m_flNextPrimaryAttack", float)
		NETVAR(next_secondary_attack, "DT_BaseCombatWeapon->m_flNextSecondaryAttack", float)
		NETVAR(clip1_count, "DT_BaseCombatWeapon->m_iClip1", int)
		NETVAR(clip2_count, "DT_BaseCombatWeapon->m_iClip2", int)
	bool is_non_aim() {
		return this->is_bomb() || this->is_knife() || this->is_grenade() == 0;
	}
	bool is_bomb() {
		return this->item_definition_index() == WEAPON_C4;
	}
	constexpr void GetEyePosition(vec3_t& eyePosition) noexcept
	{
		memory::Call<void>(this, 285, std::ref(eyePosition));
	}

	constexpr CEntity* GetObserverTarget() noexcept
	{
		return memory::Call<CEntity*>(this, 295);
	}
	bool can_see_player_pos(CEntity* player, const vec3_t& pos) {
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
	constexpr void GetAimPunch(vec3_t& aimPunch) noexcept
	{
		memory::Call<void>(this, 346, std::ref(aimPunch));
	}

	constexpr int GetWeaponType() noexcept
	{
		return memory::Call<int>(this, 455);
	}

	constexpr int GetWeaponID() noexcept
	{
		return memory::Call<int>(this, 456);
	}

	constexpr int GetWeaponAmmo() noexcept
	{
		return memory::Call<int>(this, 457);
	}

	constexpr int GetWeaponClip() noexcept
	{
		return memory::Call<int>(this, 458);
	}	

	constexpr int GetWeaponInReload() noexcept
	{
		return memory::Call<int>(this, 461);
	}

	constexpr int GetWeaponAccuracyPenalty() noexcept
	{
		return memory::Call<int>(this, 462);
	}

	constexpr int GetWeaponState() noexcept
	{
		return memory::Call<int>(this, 463);
	}

	constexpr int GetWeaponZoomLevel() noexcept
	{
		return memory::Call<int>(this, 464);
	}

	constexpr int GetWeaponOwner() noexcept
	{
		return memory::Call<int>(this, 465);
	}

	constexpr int GetWeaponHasSilencer() noexcept
	{
		return memory::Call<int>(this, 466);
	}

	constexpr int GetWeaponInZoom() noexcept
	{
		return memory::Call<int>(this, 467);
	}

	constexpr int GetWeaponPrimaryReserveAmmoCount() noexcept
	{
		return memory::Call<int>(this, 468);
	}

	constexpr int GetWeaponSecondaryReserveAmmoCount() noexcept
	{
		return memory::Call<int>(this, 469);
	}

	constexpr int GetWeaponThrowTime() noexcept
	{
		return memory::Call<int>(this, 471);
	}

	constexpr int GetWeaponPostponeFireReadyTime() noexcept
	{
		return memory::Call<int>(this, 472);
	}

	constexpr int GetWeaponLastShotTime() noexcept
	{
		return memory::Call<int>(this, 473);
	}

	constexpr float GetInaccuracy() noexcept
	{
		return memory::Call<float>(this, 483);
	}

	constexpr float GetFlashDuration() noexcept
	{
		return memory::Call<float>(this, 377);
	}

	constexpr void UpdateAccuracyPenalty() noexcept
	{
		memory::Call<void>(this, 484);
	}

	constexpr float GetSpread() noexcept
	{
		return memory::Call<float>(this, 453);
	}

};
/*
class BaseAnimating : public CEntity
{
public:

};

class BaseAnimatingOverlay : public BaseAnimating
{
public:
	NETVAR(Sequence, "CBaseAnimating->m_nSequence", unsigned)
};

class BaseFlex : public BaseAnimatingOverlay
{
public:

};

class BaseCombatCharacter : public BaseFlex
{
public:
	NETVAR(Weapons, "CBaseCombatCharacter->m_hMyWeapons", std::array<unsigned long, 48>)
};

class BasePlayer : public BaseCombatCharacter
{
public:
	//NETVAR(Flags, "CBasePlayer->m_fFlags", int)
		NETVAR(Velocity, "CBasePlayer->m_vecVelocity[0]", Vector)
		NETVAR(ViewModel, "CBasePlayer->m_hViewModel[0]", BaseHandle)
		NETVAR(TickBase, "CBasePlayer->m_nTickBase", int)
};
class CSPlayer : public BasePlayer
{
public:
	NETVAR(IsScoped, "CCSPlayer->m_bIsScoped", bool)
		NETVAR(IsDefusing, "CCSPlayer->m_bIsDefusing", bool)
		NETVAR(HasGunGameImmunity, "CCSPlayer->m_bGunGameImmunity", bool)
		NETVAR(Account, "CCSPlayer->m_iAccount", int)
		NETVAR(InBombZone, "CCSPlayer->m_bInBombZone", bool)
		NETVAR(HasDefuser, "CCSPlayer->m_bHasDefuser", bool)
		NETVAR(HasHelmet, "CCSPlayer->m_bHasHelmet", bool)
};*/