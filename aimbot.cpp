#include "aimbot.h"
#include "misc.h"

#include "src/semicore/globals.h"
#include "src/semicore/interfaces.h"
#include "src/valve/cusercmd.h"


void hacks::RunAimbot(CUserCmd* cmd) noexcept
{
	if (!v::aim.legitaim)
		return;

	if (!v::aim.aimbot)
		return;
	
	// check if we are trying to shoot
	if (!(cmd->buttons & CUserCmd::IN_ATTACK))
		return;

	if (globals::localPlayer->IsDefusing())
		return;

	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

	if (!activeWeapon)
		return;

	const int weaponType = activeWeapon->GetWeaponType();

	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_SHOTGUN:
	case CEntity::WEAPONTYPE_SNIPER:
	case CEntity::WEAPONTYPE_PISTOL:
	{
		if (!activeWeapon->GetClip())
			return;
		if (v::aim.autoscope) {
			if (weaponType == CEntity::WEAPONTYPE_SNIPER)
			{
				if (!globals::localPlayer->IsScoped())
					return;
			}
		}
		else
		break;
	}

	default:
		return;
	}

	CVector bestAngle{ };
	float bestFov = v::aim.fov;
	for (int i = 1; i <= interfaces::globals->maxClients; ++i)
	{
		CEntity* player = interfaces::entityList->GetEntityFromIndex(i);

		if (!player)
			continue;

		if (player->IsDormant() || !player->IsAlive())
			continue;

		if (player->GetTeam() == globals::localPlayer->GetTeam())
			continue;

		if (player->HasGunGameImmunity())
			continue;

		// player's bone matrix
		CMatrix3x4 bones[128];
		if (!player->SetupBones(bones, 128, 256, interfaces::globals->currentTime))
			continue;

		// our eye position
		CVector localEyePosition;
		globals::localPlayer->GetEyePosition(localEyePosition);

		// our aim punch
		CVector aimPunch{ };

		switch (weaponType)
		{
		case CEntity::WEAPONTYPE_RIFLE:
		case CEntity::WEAPONTYPE_SUBMACHINEGUN:
		case CEntity::WEAPONTYPE_MACHINEGUN:
			globals::localPlayer->GetAimPunch(aimPunch);
		}
			CTrace trace;
			interfaces::engineTrace->TraceRay(
				CRay{ localEyePosition, bones[v::aim.bone].Origin() },
				MASK_SHOT, // this is what stops it from autowalling, so if u wanna add an autowall feel free but im not doing that shit rn, i wanna smash my head on the desk already
				CTraceFilter{ globals::localPlayer }, // filter out local player  
				trace
			);

			if (!trace.entity || trace.fraction < 0.97f)
				return;

			
		
		CVector enemyAngle
		{
			(bones[v::aim.bone].Origin() - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch)
		};

		if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov)
		{
			bestFov = fov;
			bestAngle = enemyAngle;

		}


	}

	cmd->viewAngles = cmd->viewAngles + bestAngle.Scale(v::aim.scale);
}


// this curerently does not work im p sure, i have 0 clue why. i already debuged it and it makes it through the whole loop & everything so... im pretty lost lmfao. i'm thinking maybe 
// it has to do with the way i do the silent aim in hooks?? im not sure tho, take a look at it for me thanks
void hacks::RecoilControl(CUserCmd* cmd)
{

	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

	if (!activeWeapon)
		return;

	const int weaponType = activeWeapon->GetWeaponType();

	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_SHOTGUN:
	case CEntity::WEAPONTYPE_SNIPER:
	case CEntity::WEAPONTYPE_PISTOL:
	{
		if (!activeWeapon->GetClip())
			return;
		if (v::aim.autoscope) {
			if (weaponType == CEntity::WEAPONTYPE_SNIPER)
			{
				if (!globals::localPlayer->IsScoped())
					return;
			}
		}
		else
			break;
	}

	default:
		return;
	}

	CVector aimPunch{ };
	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_SUBMACHINEGUN:
	case CEntity::WEAPONTYPE_MACHINEGUN:
		globals::localPlayer->GetAimPunch(aimPunch);
	}

	if (v::aim.rcs)
	{
		static CVector lastAimPunch{ };
	
			if (cmd->buttons & CUserCmd::IN_ATTACK && aimPunch.notNull())
			{
				CVector currentPunch = aimPunch;

					cmd->viewAngles.y -= currentPunch.y * 2;
					cmd->viewAngles.x -= currentPunch.x * 2;
					lastAimPunch = CVector{ };
				
			}
		else
		{
			lastAimPunch = CVector{ };
		}
	}

}
