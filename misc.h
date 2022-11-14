#pragma once
#include "src/valve/cusercmd.h"
#include <array>
#include <map>
class CUserCmd;
namespace hacks
{
	void RunBunnyHop(CUserCmd* cmd) noexcept;
	void run_local_animations();

	float normalizeYaw(float yaw) noexcept;

	Vector getVelocity() noexcept;
	
	void runAntiaim(CUserCmd* cmd, bool& send_packet);

	void autostrafe(CUserCmd* cmd, CVector& currentViewAngles) noexcept;

	void fastStop(CUserCmd* cmd) noexcept;

	void watermark() noexcept;

	void world_stuff() noexcept;

	void revealranks(CUserCmd* cmd) noexcept;

	void HiddenCvars() noexcept;

	void RecoilControl(CUserCmd* cmd);

	void RunAimbot(CUserCmd* cmd) noexcept;

	void run_fl(bool& send_packet) noexcept;
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
};
	inline auto aim = Aim{};
}