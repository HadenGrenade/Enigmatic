#pragma once
#include "src/valve/cusercmd.h"
#include <array>
#include <map>
class CUserCmd;
namespace hacks
{
	void RunBunnyHop(CUserCmd* cmd) noexcept;

	void autostrafe(CUserCmd* cmd) noexcept;

	void RecoilControl(CUserCmd* cmd);

	void RunAimbot(CUserCmd* cmd) noexcept;
}

namespace v {
	struct Visuals
	{

		std::pair<bool, std::array<float, 4U>> player{ false, {1.f, 0.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> weapon{ false, {1.f, 1.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> grenade{ false, {1.f, 0.5f, 0.5f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> bomb{ false, {1.f, 0.f, 0.f, 1.f} };
		std::pair<bool, std::array<float, 4U>> chicken{ false, {1.f, 1.f, 0.f, 0.7f} };

		bool box = false;
		bool health = false;

	};
	inline auto visuals = Visuals{ };	
	
	struct Misc
	{
		bool bunnyHop = false;
		bool autoStrafe = false;

	};
	inline auto misc = Misc{ };

	struct Aim
	{
		bool aimbot = false;
		float fov = 5.f;
		bool silent = false;
		float scale = 0.5;
		bool autoscope = false;
		int bone = 8;
		bool legitaim = true;
		bool rcs = false;
};
	inline auto aim = Aim{};
}