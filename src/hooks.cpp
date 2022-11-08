#include "hooks.h"

#include <stdexcept>
#include <intrin.h>
#include "../ext/minhook/minhook.h"


#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"

#include "util/memory.h"
#include "semicore/interfaces.h"
#include "../misc.h"
#include "../features.h"

void hooks::Setup()
{

	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(
		memory::Get(interfaces::clientMode, 24), // CreateMove is index 24
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)));
	// PaintTraverse hook

	MH_CreateHook(
		memory::Get(interfaces::panel, 41),
		&PaintTraverse,
		reinterpret_cast<void**>(&PaintTraverseOriginal)
	);
	// DoPostScreenSpaceEffects hook
	MH_CreateHook(
		memory::Get(interfaces::clientMode, 44),
		&DoPostScreenSpaceEffects,
		reinterpret_cast<void**>(&DoPostScreenSpaceEffectsOriginal)
	);

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");



	MH_EnableHook(MH_ALL_HOOKS);
	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept
{
	// makes sure that your sorry ass can join teams

	if (cmd->commandNumber == 0)
		return false;

	if (!cmd->commandNumber)
		return CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	if (v::aim.silent) {
		// this would be done anyway by returning true
		if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);
		v::aim.scale = 1.0;

	}

	globals::UpdateLocalPlayer();
	//	bool send_packet = true;

	if (globals::localPlayer && globals::localPlayer->IsAlive())
	{
		hacks::RunBunnyHop(cmd);

		hacks::RunAimbot(cmd);
	//	hacks::RecoilControl(cmd);

	}
	if (v::aim.legitaim) {
		if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);
	}
	return false;

}

int __stdcall hooks::DoPostScreenSpaceEffects(const ViewSetup* view)
{
	f::visuals.Glow();

	return hooks::DoPostScreenSpaceEffectsOriginal(interfaces::clientMode, view);
}

void __stdcall hooks::PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept
{

	f::visuals.esp(vguiPanel, forceRepaint, allowForce);
	// call original function

	return	PaintTraverseOriginal(interfaces::panel, vguiPanel, forceRepaint, allowForce);

}



