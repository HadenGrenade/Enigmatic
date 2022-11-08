#include "visuals.h"
#include "iglowmanger.h"
#include "src/hooks.h"
#include "misc.h"
#include <string>
void Visuals::Glow()
{
	if (!globals::localPlayer)
		return;

	for (auto i = 0; i < interfaces::glow->glowObjectDefinitions.Count(); ++i)
	{
		auto& glow = interfaces::glow->glowObjectDefinitions[i];

		if (glow.IsUnused())
			continue;

		if (!glow.entity)
			continue;

		const auto clientClass = glow.entity->GetClientClass();

		if (!clientClass)
			continue;

		switch (clientClass->classID)
		{
		case CClientClass::CCSPlayer:
		{
			if (!v::visuals.player.first)
				break;
				///cplayer
			const auto player = static_cast<CEntity*>(glow.entity);

			if (player->GetTeam() == globals::localPlayer->GetTeam())
				break;
			if (!player->IsAlive())
				break;

			glow.SetColor(v::visuals.player.second.data());
			break;
		}

		case CClientClass::CChicken:
			if (!v::visuals.chicken.first)
				break;

			glow.SetColor(v::visuals.chicken.second.data());
			break;

		case CClientClass::CC4:
		case CClientClass::CPlantedC4:
			if (!v::visuals.bomb.first)
				break;

			glow.SetColor(v::visuals.bomb.second.data());
			break;

		case CClientClass::CBaseCSGrenadeProjectile:
		case CClientClass::CBreachChargeProjectile:
		case CClientClass::CBumpMineProjectile:
		case CClientClass::CDecoyProjectile:
		case CClientClass::CMolotovProjectile:
		case CClientClass::CSensorGrenadeProjectile:
		case CClientClass::CSmokeGrenadeProjectile:
			if (!v::visuals.grenade.first)
				break;

			glow.SetColor(v::visuals.grenade.second.data());
			break;

		default:
			if (!v::visuals.weapon.first)
				break;

			if (!glow.entity->IsWeapon())
				break;

			glow.SetColor(v::visuals.weapon.second.data());
			break;
		}
	}
}