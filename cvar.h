#pragma once
#include "src/util/memory.h"

struct ConVar;

class Cvar {
public:

     ConVar* FindVar(const char* name) noexcept
	{
		return memory::Call<ConVar*>(this, 15, name);
	}

};

class conCommandBase
{
public:
	void* vmt;
	conCommandBase* next;
	bool registered;
	const char* name;
	const char* helpString;
	int flags;
	conCommandBase* conCommandBases;
	void* accessor;
};