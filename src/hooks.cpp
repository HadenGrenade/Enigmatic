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
	/*
	MH_CreateHook(
		memory::Get(interfaces::engine->GetBSPTreeQuery(), 6),
		&ListLeavesInBox,
		reinterpret_cast<void**>(&ListLeavesInBoxOriginal)
	);*/
	/*
	// DrawModel hook
	if(MH_CreateHook(
		memory::Get(interfaces::studioRender, 29),
		&DrawModel,
		reinterpret_cast<void**>(&DrawModelOriginal))
	);
	*/
	if (MH_CreateHook(
		memory::Get(interfaces::clientMode, 24), // CreateMove is index 24
		&proxy,
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
}/*
int __stdcall hooks::ListLeavesInBox(const CVector& mins, const CVector& maxs, std::uint16_t* list, int listMax) noexcept
{
	static const auto insertIntoTree = reinterpret_cast<std::uintptr_t>(memory::insertIntoTree);

	if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) != insertIntoTree)
		return ListLeavesInBoxOriginal(interfaces::engine->GetBSPTreeQuery(), mins, maxs, list, listMax);

	// get the RenderableInfo pointer from the stack
	const auto info = *reinterpret_cast<CRenderableInfo**>(reinterpret_cast<std::uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	// make sure it is valid
	if (!info || !info->renderable)
		return ListLeavesInBoxOriginal(interfaces::engine->GetBSPTreeQuery(), mins, maxs, list, listMax);

	// get the base entity from the IClientRenderable pointer
	CEntity* entity = info->renderable->GetIClientUnknown()->GetBaseEntity();

	// make sure it is valid
	if (!entity || !entity->IsPlayer())
		return ListLeavesInBoxOriginal(interfaces::engine->GetBSPTreeQuery(), mins, maxs, list, listMax);

	// no need to reorder for teammates
	if (entity->GetTeam() == globals::localPlayer->GetTeam())
		return ListLeavesInBoxOriginal(interfaces::engine->GetBSPTreeQuery(), mins, maxs, list, listMax);

	// set the flags
	info->flags &= ~RENDER_FLAGS_FORCE_OPAQUE_PASS;
	info->flags2 |= RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE;

	constexpr float maxCoord = 16384.0f;
	constexpr float minCoord = -maxCoord;

	constexpr CVector min{ minCoord, minCoord, minCoord };
	constexpr CVector max{ maxCoord, maxCoord, maxCoord };

	// return with maximum bounds
	return ListLeavesInBoxOriginal(interfaces::engine->GetBSPTreeQuery(), min, max, list, listMax);
}*/
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

void __declspec(naked) hooks::proxy() {
	__asm {
		push ebp
		mov  ebp, esp

		push ebx			// Move sendpacket to stack

		push esp			// sendpacket ptr
		push[ebp + 0xC]		// usercmd
		push[ebp + 0x8]		// input_sample_frametime, already in xmm0
		//movss xmm0, [ebp + 0x8] 
		call hooks::CreateMove
		add esp, 0xC

		pop ebx				// Move sendpacket back to ebx

		mov esp, ebp
		pop ebp

		ret 0x8				// thiscall stack cleanup
	}
}
bool __stdcall hooks::CreateMove(float input_sample_frametime, CUserCmd* cmd, bool& send_packet) noexcept {

	auto currentViewAngles{ cmd->viewAngles };
	// makes sure that your sorry ass can join teams

	if (cmd->commandNumber == 0)
		return false;

	if (!cmd->commandNumber)
		return CreateMoveOriginal(interfaces::clientMode, input_sample_frametime, cmd);

	if (v::aim.silent) {
		// this would be done anyway by returning true
		if (CreateMoveOriginal(interfaces::clientMode, input_sample_frametime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);
		v::aim.scale = 1.0;

	}
	auto old_viewangles = cmd->viewAngles;
	auto old_forwardmove = cmd->forwardMove;
	auto old_sidemove = cmd->sideMove;
	globals::UpdateLocalPlayer();
	//	bool send_packet = true;

	if (globals::localPlayer && globals::localPlayer->IsAlive())
	{
		hacks::RunBunnyHop(cmd);
		hacks::autostrafe(cmd, currentViewAngles);
		hacks::RunAimbot(cmd);
		memes::bsod();
		hacks::runAntiaim(cmd, send_packet);
		hacks::run_fl(send_packet);
		//memes::minecraft();
		hacks::RecoilControl(cmd);

	}
	if (v::aim.legitaim) {
		if (CreateMoveOriginal(interfaces::clientMode, input_sample_frametime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);
	}


	cmd->forwardMove = std::clamp(cmd->forwardMove, -450.0f, 450.0f);
	cmd->sideMove = std::clamp(cmd->sideMove, -450.0f, 450.0f);
	cmd->upMove = std::clamp(cmd->upMove, -320.0f, 320.0f);

	cmd->viewAngles.normalize();
	cmd->viewAngles.clamp();
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

	return	hooks::PaintTraverseOriginal(interfaces::panel, vguiPanel, forceRepaint, allowForce);
}
/*
void __stdcall hooks::DrawModel(
	void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWeights,
	const CVector& modelOrigin,
	const std::int32_t flags) noexcept
{

	f::visuals.chams(results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
	return hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}


*/