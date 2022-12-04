#pragma once
#include "../util/memory.h"
#include "../../cmatrix.h"
#include "../../ispatialquery.h"


class IVEngineClient
{
public:
	constexpr void GetScreenSize(std::int32_t& width, std::int32_t& height) noexcept
	{
		memory::Call<void>(this, 5, std::ref(width), std::ref(height));
	}
	
	constexpr std::int32_t get_net_channel_info() noexcept
	{
		return memory::Call<std::int32_t>(this, 78);
	}

	// returns index of local player
	constexpr std::int32_t GetLocalPlayerIndex() noexcept
	{
		return memory::Call<std::int32_t>(this, 12);
	}

	constexpr void SetViewAngles(const vec3_t& viewAngles) noexcept
	{
		memory::Call<void, const vec3_t&>(this, 19, viewAngles);
	}

	// returns true if in-game
	constexpr bool IsInGame() noexcept
	{
		return memory::Call<bool>(this, 26);
	}

	constexpr bool IsConnected() noexcept
	{
		return memory::Call<bool>(this, 27);
	}

	constexpr bool IsDrawingLoadingImage() noexcept
	{
		return memory::Call<bool>(this, 28);
	}

	constexpr void GetViewAngles(CVector& viewAngles) noexcept
	{
		memory::Call<void>(this, 18, std::ref(viewAngles));
	}

	constexpr void GetPlayerInfo(std::int32_t playerIndex, void* playerInfo) noexcept
	{
		memory::Call<void>(this, 8, playerIndex, playerInfo);
	}

	constexpr void GetScreenAspectRatio(std::int32_t& width, std::int32_t& height) noexcept
	{
		memory::Call<void>(this, 101, std::ref(width), std::ref(height));
	}

	constexpr const CMatrix4x4& WorldToScreenMatrix() noexcept
	{
		return memory::Call<const CMatrix4x4&>(this, 37);
	}

	constexpr ISpacialQuery* GetBSPTreeQuery() noexcept
	{
		return memory::Call<ISpacialQuery*>(this, 43);
	}

	constexpr void ExecuteClientCmd(const char* command) noexcept
	{
		memory::Call<void, const char*>(this, 108, command);
	}

	constexpr void ClientCmd_Unrestricted(const char* command) noexcept
	{
		memory::Call<void, const char*>(this, 114, command);
	}

	constexpr bool ConIsVisible() noexcept
	{
		return memory::Call<bool>(this, 11);
	}
	

};