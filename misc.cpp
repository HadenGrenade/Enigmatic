#include "misc.h"
// access global variables

// access interfaces
#include "src/semicore/interfaces.h"
#include "src/semicore/globals.h"
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

void hacks::autostrafe(CUserCmd* cmd, CVector& currentViewAngles) noexcept
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

