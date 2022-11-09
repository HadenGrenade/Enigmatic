#pragma once
#include "src/util/memory.h"

class IVPanel
{
public:
	constexpr const char* GetName(std::uint32_t panel) noexcept
	{
		return memory::Call<const char*>(this, 36, panel);
	}

	constexpr std::uint32_t GetPanel(std::uint32_t type) noexcept
	{
		return memory::Call<std::uint32_t>(this, 37, type);
	}

	constexpr void SetMouseInputEnabled(std::uint32_t panel, bool state) noexcept
	{
		return memory::Call<void>(this, 32, panel, state);
	}


};