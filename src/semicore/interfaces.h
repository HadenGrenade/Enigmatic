#pragma once
#include "globals.h"

// interfaces
#include "../valve/icliententitylist.h"
#include "../../isurface.h"
#include "../../imaterialsystem.h"
#include "../../istudiorender.h"
#include "../valve/ivengineclient.h"
#include "../../ivpanel.h"
#include "../../ivmodelinfo.h"
#include "../../ivdebugoverlay.h"
#include "../../iglowmanger.h"
#include "../../iglobalvars.h"
#include "../../ienginevgui.h"
#include "../../ienginetrace.h"
#include "../../iclientmodeshared.h"
#include "../../ibaseclientdll.h"


// learn more about interfaces -> https://www.youtube.com/watch?v=C0wGdwnaArA
namespace interfaces
{
	// call once to capture all interface pointers
	void Setup() noexcept;

	// capture an individual interface by name & module
	template <typename Interface>
	Interface* Capture(const char* moduleName, const char* interfaceName) noexcept;

	// interface pointers
	inline IBaseClientDLL* client = nullptr;
	inline IClientEntityList* entityList = nullptr;
	inline IClientModeShared* clientMode = nullptr;
	inline IEngineTrace* engineTrace = nullptr;
	inline IEngineVGui* engineVGui = nullptr;
	inline IGlobalVars* globals = nullptr;
	inline GlowObjectManager* glow = nullptr;
	inline IMaterialSystem* materialSystem = nullptr;
	inline IStudioRender* studioRender = nullptr;
	inline ISurface* surface = nullptr;
	inline IVDebugOverlay* debugOverlay = nullptr;
	inline IVEngineClient* engine = nullptr;
	inline IVModelInfo* modelInfo = nullptr;
	inline IVPanel* panel = nullptr;

	// other
	inline void* keyValuesSystem = nullptr;
}
