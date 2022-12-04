#pragma once
#include "clientmode.h"

//dont need this for now and prob never will so its gonna b empty, if u got a problem with it do it urself :P

class IClientModeShared
{
public:

	auto getHudChat() noexcept
	{
		return *reinterpret_cast<HudChat**>(std::uintptr_t(this) + 28);
	}

};