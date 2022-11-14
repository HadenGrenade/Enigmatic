#pragma once

#include "cclientclass.h"
#include "src/util/memory.h"

class IBaseClientDLL
{
public:
	constexpr CClientClass* GetAllClasses() noexcept
	{
		return memory::Call<CClientClass*>(this, 8);
	}

	constexpr bool writeUsercmdDeltaToBuffer() noexcept
	{
		return memory::Call<bool>(this, 24);
	}

	constexpr bool dispatchUserMessage(int messageType, int arg, int arg1, void* data) noexcept
	{
		return memory::Call<bool>(this, 38, this, messageType, arg, arg1, data);
	}

};