#include "interfaces.h"
#include "../util/memory.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
void interfaces::Setup() noexcept
{
	// capture the interfaces

	surface_props_physics   = Capture<physics_surface_props>("vphysics.dll", "VPhysicsSurfaceProps001");
	client				 	= Capture<IBaseClientDLL>("client.dll", "VClient018");
	entityList			    = Capture<ClientEntityList>("client.dll", "VClientEntityList003");
	clientMode		     	= **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
	engineTrace		     	= Capture<IEngineTrace>("engine.dll", "EngineTraceClient004");
	engineVGui			    = Capture<IEngineVGui>("engine.dll", "VEngineVGui001");
	globals					= **reinterpret_cast<IGlobalVars***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
 	glow				    =*reinterpret_cast<GlowObjectManager**>(memory::glowObjectManager);
	materialSystem      	= Capture<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	studioRender 	     	= Capture<IStudioRender>("studiorender.dll", "VStudioRender026");
	surface		        	= Capture<ISurface>("vguimatsurface.dll", "VGUI_Surface031");
	debugOverlay	        = Capture<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
	engine	             	= Capture<IVEngineClient>("engine.dll", "VEngineClient014");
	modelInfo       		= Capture<IVModelInfo>("engine.dll", "VModelInfoClient004");
	panel		            = Capture<IVPanel>("vgui2.dll", "VGUI_Panel009");
	input				    =*reinterpret_cast<i_input**>(memory::PatternScan("client.dll", sig_input) + 1);
	prediction				= Capture<player_prediction>("client.dll", "VClientPrediction001");
	cvar					= Capture<Cvar>("vstdlib.dll", "VEngineCvar007");
	weapon_system           = *reinterpret_cast<i_weapon_system**>(memory::PatternScan("client.dll", sig_weapon_data) + 2);

	// get the exported KeyValuesSystem function
	if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))
		// set our pointer by calling the function
		keyValuesSystem = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();
}

template <typename Interface>
Interface* interfaces::Capture(const char* moduleName, const char* interfaceName) noexcept
{
	const HINSTANCE handle = GetModuleHandle(moduleName);

	if (!handle)
		return nullptr;

	// get the exported Createinterface function
	using CreateInterfaceFn = Interface * (__cdecl*)(const char*, int*);
	const CreateInterfaceFn createInterface =
		reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

	// return the interface pointer by calling the function
	return createInterface(interfaceName, nullptr);
}
