#pragma once
#include "src/valve/cusercmd.h"
#include <array>
#include <map>
class CUserCmd;
namespace hacks
{
	void RunBunnyHop(CUserCmd* cmd) noexcept;


}

namespace v {
	struct Visuals
	{

		std::pair<bool, std::array<float, 4U>> player{ false, {1.f, 0.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> weapon{ false, {1.f, 1.f, 1.f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> grenade{ false, {1.f, 0.5f, 0.5f, 0.7f} };
		std::pair<bool, std::array<float, 4U>> bomb{ false, {1.f, 0.f, 0.f, 1.f} };
		std::pair<bool, std::array<float, 4U>> chicken{ false, {1.f, 1.f, 0.f, 0.7f} };
	};
	inline auto visuals = Visuals{ };	struct Misc
	{
		bool bunnyHop = false;


	};
	inline auto misc = Misc{ };
}