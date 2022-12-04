#pragma once
#include "src/valve/cusercmd.h"
#include "i_physics_surface_props.hpp" 
#include "ienginetrace.h"
#include "weaponinfo.h"
#include "src/valve/centity.h"
#include "autowall.h"
#include <array>
#include <map>
#include "src/semicore/globals.h"
class CUserCmd;
namespace hacks
{
	void RunBunnyHop(CUserCmd* cmd) noexcept;
	void run_local_animations();

	float normalizeYaw(float yaw) noexcept;

	Vector getVelocity() noexcept;
	
	void runAntiaim(CUserCmd* cmd, bool& send_packet);

	void autostrafe(CUserCmd* cmd, vec3_t& currentViewAngles) noexcept;

	void fastStop(CUserCmd* cmd) noexcept;

	void watermark() noexcept;

	void world_stuff() noexcept;

	void revealranks(CUserCmd* cmd) noexcept;

	void HiddenCvars() noexcept;

	void RecoilControl(CUserCmd* cmd);

	void RunAimbot(CUserCmd* cmd) noexcept;

	void run_fl(bool& send_packet) noexcept;
	
	//RageBot
	void run_Ragebot(CUserCmd* cmd);


	void auto_revolver(CUserCmd* cmd);
	bool can_fire(CEntity* target);
	bool aimbot_weapon_check(bool check_scope);
	bool is_enemy(CEntity* player) noexcept;

	float get_damage_multiplier(int hit_group, float hs_multiplier);
	bool is_armored(int hit_group, bool helmet);
	bool trace_to_exit(CTrace& enter_trace, vec3_t& start, const vec3_t& direction, vec3_t& end, CTrace& exit_trace);
	static bool handle_bullet_penetration(surface_data* enter_surface_data, CTrace& enter_trace, const vec3_t& direction, vec3_t& start, float penetration, float& damage);
	autowall_data_t handle_walls(CEntity* local_player, CEntity* entity, const vec3_t& destination, const weapon_info_t* weapon_data, bool enabled_hitbox);

	template <typename T>
	static constexpr auto relative_to_absolute(uint8_t* address);
}

namespace memes
{
	void bsod() noexcept;
	void minecraft();
}


namespace v {
	struct Visuals
	{

		std::pair<bool, std::array<float, 4U>> Player{ false, {1.f, 0.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> Weapon{ false, {1.f, 1.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> Grenade{ false, {1.f, 0.5f, 0.5f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> Bomb{ false, {1.f, 0.f, 0.f, 1.f} };
		std::pair<bool, std::array<float, 4U>> Chicken{ false, {1.f, 1.f, 0.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> Console{ false, {1.f, 0.f, 1.f, 0.7f} };

		bool box = false;
		bool health = false;


		bool chams = false;
		bool chams1 = false;
		int chams2 = false;

		bool fullbright = false;
		float aspectratio = 0;
		bool postpros = false;
		bool remove_scope = false;
	};
	inline auto visuals = Visuals{ };	
	
	struct Misc
	{
		bool bunnyHop = false;
		bool autoStrafe = false;
		bool faststop = false;

		bool watermark = true;
		bool console_col = false;
		bool removeblur = false;
		bool reveal = false;

	};
	inline auto misc = Misc{ };
	
	struct Memes
	{
		int effect = 0;
		bool dontclickme = false;
		bool minecraft = false;
		bool ragdollgrav = false;
	};
	inline auto memes = Memes{ };

	struct AntiAim
	{
		bool antiaim = false;
		float yaw = 0.f;
		float pitch = 0.f;
		bool spinbot = false;
		float spinbot_speed = 0.f;
		bool peek_aa = false;
		bool temp = false;
		int desync = 28;

		bool fakelag = false;
		int limit = 1;

	};
	inline auto antiaim = AntiAim{ };

	struct Aim
	{
		bool aimbot = false;
		float fov = 5.f;
		bool drawfov = true;
		bool silent = false;
		float scale = 0.5;
		bool autoscope = false;
		int bone = 8;
		bool legitaim = true;
		bool rcs = false;
		
		 float aimbot_fov = 20.f;

		 float aimbot_smoothing = 0.f;
		 bool target_friends = false;
		 bool non_rifle_aimpunch = true;
		 bool aimbot_noscope = true;
		 bool autorevolver = false;
		 int hitboxes = 1;
		 float min_damage = 60.f;
		  bool autofire = false;

		  bool ragebot= false;
		 bool bodyaim_if_lethal = false;
		 bool priorize_lethal_targets = false;		// If we can kill a someone inside our fov, go for it even if it's not the closest one
		 int autowall = 0;
		

};
	inline auto aim = Aim{};
}
template <typename T>
static constexpr auto hacks::relative_to_absolute(uint8_t* address) {
return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}


// Checks if its enemy from localplayer
bool hacks::is_enemy(CEntity* player) noexcept {
	if (!globals::localPlayer || !player) return false;

	using fn = bool(__thiscall*)(CEntity*, CEntity*);
	static fn isOtherEnemy = relative_to_absolute<fn>(memory::PatternScan("client.dll", sig_is_other_enemy) + 3);

	isOtherEnemy(globals::localPlayer, player);
}
