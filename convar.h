#pragma once

#include "src/util/memory.h"

struct ConVar {

     float getfloat() noexcept
    {
		return memory::Call<float>(this, 12);
    }

	 int getint() noexcept
	{
		return memory::Call<int>(this, 13);
	}

	 void setvalue(const char* value) noexcept
	{
		memory::Call<void>(this, 14, value);
	}
	
	 void setvalue(float value) noexcept
	{
		memory::Call<void>(this, 15, value);
	}

	 void setvalue(int value) noexcept
	{
		memory::Call<void>(this, 16, value);
	}

  
};
