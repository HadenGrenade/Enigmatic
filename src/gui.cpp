#include "gui.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../misc.h"
#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);


bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,            //width but u dont rlly see this so it sorta kinda dont matter
		100,   
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;  //check sucess daddyu :moan:

	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("YoMomma"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{



	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
		);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = { 15, 15 };
	style->WindowRounding = 0.f;
	style->WindowTitleAlign = { 0.5f, 0.5f };
	style->WindowBorderSize = 1;

	style->FrameRounding = 0.f;
	style->FramePadding = { 20, 7 };
	style->FrameBorderSize = 1;

	style->ChildRounding = 0.f;

	style->ScrollbarSize = 3.f;
	style->ScrollbarRounding = 1.f;

	style->ItemSpacing = { 8, 8 };

	style->TabRounding = 0.f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.96f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.14f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.32f, 0.32f, 0.58f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.52f, 0.26f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.38f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.37f, 0.62f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.33f, 0.33f, 0.68f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.81f, 0.66f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.35f, 0.37f, 0.48f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.35f, 0.49f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.42f, 0.32f, 0.67f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.41f, 0.73f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.50f, 1.00f, 0.35f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.29f, 0.84f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.24f, 0.80f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs
		| ImGuiColorEditFlags_NoLabel
		| ImGuiColorEditFlags_NoSidePreview
		| ImGuiColorEditFlags_Uint8
		| ImGuiColorEditFlags_AlphaBar);
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	segoe = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.f);

	setup = true;
}
void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// retore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}
static bool toggle = false;

static auto Name = "GrenadeV1";
static auto Flags = ImGuiWindowFlags_NoTitleBar
//| ImGuiWindowFlags_NoResize
//| ImGuiWindowFlags_NoCollapse
| ImGuiWindowFlags_NoSavedSettings
//| ImGuiWindowFlags_NoScrollbar
| ImGuiWindowFlags_NoScrollWithMouse;

void HelpMarker(const char* desc)
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

#define VISUALS_COLOR_BOOL(name) \
ImGui::Checkbox(#name, &v::visuals.name.first); \
ImGui::SameLine(offset); \
ImGui::ColorEdit4(#name, v::visuals.name.second.data())


void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//cool fx shit
	gui::FxTestBed();

	const auto offset = 195.f;

	ImGuiStyle* style = &ImGui::GetStyle();

	ImGui::PushFont(segoe);
	ImGui::SetNextWindowSize({ 512, 500 });
	ImGui::Begin("enigmatic", &open);
	static auto tab = 0;
	const auto avail = ImGui::GetContentRegionAvail();
	const auto tabSize = ImVec2(90, 40);
	if (ImGui::Button("Rage", tabSize)) { tab = 0; }
	ImGui::SameLine();
	if (ImGui::Button("Legit", tabSize)) { tab = 1; }
	ImGui::SameLine();
	if (ImGui::Button("Visuals", tabSize)) { tab = 2; }
	ImGui::SameLine();
	if (ImGui::Button("Misc", tabSize)) { tab = 3; }
	ImGui::SameLine();
	if (ImGui::Button("Memes", tabSize)) { tab = 4; }
	switch (tab)
	{
	case 0:
		ImGui::BeginGroup();
		ImGui::BeginChild(1, { (avail.x - 8) * 0.5f, avail.y * 0.65f }, true);
		ImGui::Text("Coming soon...");
		HelpMarker("Hit P on Some Nigga's");
		ImGui::Separator();
		ImGui::Spacing();

		//insert your p code pysik

		ImGui::EndChild();


		ImGui::BeginChild(2, { (avail.x - 8) * 0.5f, ImGui::GetContentRegionAvail().y }, true);
		ImGui::Text("AntiAim");
		HelpMarker("Missed due to resolver - Every Cheat $$");
		ImGui::Separator();
		ImGui::Spacing();

		//insert your p code pysik

		ImGui::EndChild();
		ImGui::EndGroup();
		break;
	case 1:

		ImGui::BeginGroup();
		ImGui::BeginChild(1, { (avail.x - 8) * 0.5f, (avail.y * 0.4f) }, true);
		ImGui::Text("LegitBot:");
		HelpMarker("Faceit Protected $$");
		ImGui::Checkbox("Legitbot", &v::aim.aimbot);
		if (v::aim.aimbot) {
			ImGui::Checkbox("Legit Aim", &v::aim.legitaim);
			HelpMarker("Please only have one of these active at a time, as it can cause errors ");
			ImGui::Checkbox("Silent Aim", &v::aim.silent);
			HelpMarker("Enemys still can see the flick, however you cannot. ");
			ImGui::Checkbox("AutoScope", &v::aim.autoscope);
			HelpMarker(" If you have a sniper it will scope at the enemy when you shoot. ");
		}
		ImGui::EndChild();

		ImGui::BeginChild(2, { (avail.x - 8) * 0.5f, (avail.y * 0.33f) }, true);
		ImGui::Text("Legit AA:");
		HelpMarker("Confuse some niggas");
		ImGui::Separator();

		ImGui::EndChild();

		ImGui::BeginChild(3, { (avail.x - 8) * 0.5f, ImGui::GetContentRegionAvail().y }, true);
		ImGui::Text("Recoil:");
		HelpMarker("Controls your recoil for you. ");
	//	ImGui::Checkbox("Recoil", &v::aim.rcs);
		ImGui::Separator();
		ImGui::EndChild();
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::BeginChild(4, { (avail.x - 8) * 0.5f, 0.3f }, true);
		ImGui::Text("Super Legit");
		HelpMarker(" Modifications to make you look less like a cheater ! ");
		if (v::aim.aimbot) {
			ImGui::SliderInt("Bone Selection", &v::aim.bone, 1, 8, "%1f bone");
			HelpMarker(" Based on the number you select, it will choose that bone. 8 being head and 1 being feet. ");


			ImGui::Separator();
			if (!v::aim.silent)
			{
				ImGui::SliderFloat("Smoothness", &v::aim.scale, 0.0f, 1.0f, "%.1f smoothness");
				HelpMarker("Changes the time it will take for your crosshair to reach their head ");
			}
			HelpMarker("Changes the time it will take for your crosshair to reach their head ");
			ImGui::SliderFloat("FOV", &v::aim.fov, 0.1f, 180.0f, "%1f fov");
			HelpMarker("Changes the distance your crosshair has to be from the player for the cheat to recognize them. ");
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		break;
	case 2:
	{
		ImGui::BeginGroup();
		ImGui::BeginChild(1, { (avail.x - 8) * 0.5f, avail.y }, true);
		ImGui::Text("Visuals:");
		HelpMarker("See the opps b4 they c u $");
		ImGui::Separator();
		VISUALS_COLOR_BOOL(player);
		VISUALS_COLOR_BOOL(weapon);
		VISUALS_COLOR_BOOL(grenade);
		VISUALS_COLOR_BOOL(bomb);
		VISUALS_COLOR_BOOL(chicken);

		ImGui::EndChild();
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::BeginChild(2, { (avail.x - 8) * 0.5f, avail.y }, true);
		ImGui::Text("Surface ESP:");
		HelpMarker("Gives information about an enemy using a Directx9 hook");
		ImGui::Separator();
		ImGui::Checkbox("Box ESP", &v::visuals.box);
		ImGui::Checkbox("Health ESP", &v::visuals.health);

		ImGui::EndChild();
		ImGui::EndGroup();
	} break;

	case 3:
	{
		ImGui::BeginGroup();
		ImGui::BeginChild(1, { (avail.x - 8) * 0.5f, avail.y }, true);
		ImGui::Text("Movement");
		ImGui::Separator();
		ImGui::Checkbox("BunnyHop", &v::misc.bunnyHop);
		ImGui::Checkbox("AutoStrafe", &v::misc.autoStrafe);
		ImGui::EndChild();
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::BeginChild(2, { (avail.x - 8) * 0.5f, (avail.y - 8) * 0.72f }, true);
		ImGui::Text("Selected:");

		ImGui::Separator();


		ImGui::EndChild();

		ImGui::BeginChild(3, { }, true);


		ImGui::EndChild();
		ImGui::EndGroup();
	}break;
	case 4:
	{
		ImGui::BeginGroup();
		ImGui::BeginChild(1, { (avail.x - 8) * 0.5f, avail.y }, true);
		ImGui::Text("Memes:");
		ImGui::Checkbox("Don't click me (seriously)", &v::memes.dontclickme);

		ImGui::EndChild();
		ImGui::EndGroup();
	} break;
	}
		


	ImGui::End();
	ImGui::PopFont();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toogle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}
