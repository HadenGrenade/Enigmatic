#include "misc.h"
#include "ext/imgui/imgui.h"
#include "ext/imgui/imgui_internal.h"
// access global variables

// access interfaces
#include "src/semicore/ainterfaces.h"
#include "src/semicore/globals.h"


#include "convar.h"
#include <string>

void hacks::RunBunnyHop(CUserCmd* cmd) noexcept
{
	if (!v::misc.bunnyHop)
		return;
	//CEntity::FL_ONGROUND
		if (!(globals::localPlayer->GetFlags() & 1))
			cmd->buttons &= ~CUserCmd::IN_JUMP;
	
	
}
// ripped this bitch from uc  --https://www.unknowncheats.me/forum/counterstrike-global-offensive/144026-basic-auto-strafe.html
//it doesnt work, no clue why, not fixing it, feel free to pysik! 

float hacks::normalizeYaw(float yaw) noexcept
{

	if (!std::isfinite(yaw))
		return 0.0f;

	if (yaw >= -180.f && yaw <= 180.f)
		return yaw;

	const float rot = std::round(std::abs(yaw / 360.f));
	
	yaw = (yaw < 0.f) ? yaw + (360.f * rot) : yaw - (360.f * rot);
	
	return yaw;

}

Vector hacks::getVelocity() noexcept
{
	return globals::localPlayer->velocity();
}

void hacks::autostrafe(CUserCmd* cmd, vec3_t& currentViewAngles) noexcept
{
	if (!v::misc.autoStrafe)
		return;

	const float speed = globals::localPlayer->velocity().Length2D();
	if (speed < 5.0f)
		return;

	static float angle = 0.f;

	const bool back = cmd->buttons & CUserCmd::IN_BACK;
	const bool forward = cmd->buttons & CUserCmd::IN_FORWARD;
	const bool right = cmd->buttons & CUserCmd::IN_MOVERIGHT;
	const bool left = cmd->buttons & CUserCmd::IN_MOVELEFT;

	if (back) {
		angle = -180.f;
		if (left)
			angle -= 45.f;
		else if (right)
			angle += 45.f;
	}
	else if (left) {
		angle = 90.f;
		if (back)
			angle += 45.f;
		else if (forward)
			angle -= 45.f;
	}
	else if (right) {
		angle = -90.f;
		if (back)
			angle -= 45.f;
		else if (forward)
			angle += 45.f;
	}
	else {
		angle = 0.f;
	}
	
	//check if on ground
	if ((globals::localPlayer->GetFlags()) & 1)
		return;
	
	currentViewAngles.y += angle;

	cmd->forwardMove = 0.f;
	cmd->sideMove = 0.f;

	const auto delta = normalizeYaw(currentViewAngles.y - Vector::Rad2Deg(std::atan2(getVelocity().y, getVelocity().x)));

	cmd->sideMove = delta > 0.f ? -450.f : 450.f;

	currentViewAngles.y = normalizeYaw(currentViewAngles.y - delta);

 }

void hacks::fastStop(CUserCmd* cmd) noexcept
{
	if (!v::misc.faststop)
		return;

	if (cmd->buttons & CUserCmd::IN_JUMP)
		return;

	//check if on ground
	if (!((globals::localPlayer->GetFlags()) & 1))
		return;

	if (cmd->buttons & (CUserCmd::IN_MOVELEFT | CUserCmd::IN_MOVERIGHT | CUserCmd::IN_FORWARD | CUserCmd::IN_BACK))
		return;

	const auto velocity = globals::localPlayer->velocity();
	const auto speed = velocity.Length2D();
	if (speed < 15.0f)
		return;

	Vector direction = velocity.ToAngle();
	direction.y = cmd->viewAngles.y - direction.y;

	const auto negatedDirection = Vector::FromAngle(direction) * -speed;
	cmd->forwardMove = negatedDirection.x;
	cmd->sideMove = negatedDirection.y;
}

void hacks::watermark() noexcept
{
	if (!v::misc.watermark)
		return;

	int x, y;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

	interfaces::engine->GetScreenSize(x, y);
	
	ImGui::SetNextWindowPos(ImVec2(x - 80, 30), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::Begin("Watermark", NULL, windowFlags);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.0f;
	style.WindowBorderSize = 1.0f;

	static auto frameRate = 1.0f;
	frameRate = 0.9f * frameRate + 0.1f * interfaces::globals->absFrameTime;

	ImGui::Text("enigmatic | %d fps ", frameRate != 0.0f ? static_cast<int>(1 / frameRate) : 0);
	ImGui::End();
}

void hacks::world_stuff() noexcept
{
	
	static auto cheats = interfaces::cvar->FindVar("sv_cheats");
	cheats->setvalue(1);

	static auto bright = interfaces::cvar->FindVar("mat_fullbright");
	bright->setvalue(v::visuals.fullbright);
	
	static auto aspect = interfaces::cvar->FindVar("r_aspectratio");
	aspect->setvalue(v::visuals.aspectratio);
	
	static auto blur = interfaces::cvar->FindVar("@panorama_disable_blur");
	blur->setvalue(v::misc.removeblur);

	static auto ragdollgrav = interfaces::cvar->FindVar("cl_ragdoll_gravity");
	ragdollgrav->setvalue(v::memes.ragdollgrav ? -600 : 600);

	static auto minecraft = interfaces::cvar->FindVar("mat_showlowresimage");
	minecraft->setvalue(v::memes.minecraft);

	static auto post_proc = interfaces::cvar->FindVar("mat_postprocess_enable");
	post_proc->setvalue(v::visuals.postpros ? 0 : 1);
	
}

void hacks::revealranks(CUserCmd* cmd) noexcept
{
	if (v::misc.reveal && cmd->buttons & CUserCmd::IN_SCORE)
		interfaces::client->dispatchUserMessage(50, 0, 0, nullptr);
}

void hacks::HiddenCvars() noexcept //https://chng.it/MQbQhn44Jk
{
	auto iterator = **reinterpret_cast<conCommandBase***>(interfaces::cvar + 0x34);
	for (auto c = iterator->next; c != nullptr; c = c->next)
	{
		conCommandBase* cmd = c;
		cmd->flags &= ~(1 << 1);
		cmd->flags &= ~(1 << 4);
	}
}



// Checks if its enemy from localplayer
bool hacks::is_enemy(CEntity* player) noexcept {
	if (!globals::localPlayer || !player) return false;

	using fn = bool(__thiscall*)(CEntity*, CEntity*);
	static fn isOtherEnemy = relative_to_absolute<fn>(memory::PatternScan("client.dll", sig_is_other_enemy) + 3);

	isOtherEnemy(globals::localPlayer, player);
}

