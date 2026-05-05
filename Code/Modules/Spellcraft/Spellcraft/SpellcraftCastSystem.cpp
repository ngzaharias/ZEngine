#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftCastSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Spellcraft/SpellcraftBookComponent.h"
#include "Spellcraft/SpellcraftCastComponent.h"
#include "Spellcraft/SpellcraftExplosionComponent.h"
#include "Spellcraft/SpellcraftExplosionHitEvent.h"
#include "Spellcraft/SpellcraftProjectileComponent.h"
#include "Spellcraft/SpellcraftProjectileHitEvent.h"

void spellcraft::CastSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& hitEvent : world.Events<spellcraft::ExplosionHitEvent>())
	{
		if (!hitEvent.m_OnHit.IsValid())
			continue;

		const ecs::Entity entity = world.CreateEntity();
		auto& castComponent = world.AddComponent<spellcraft::CastComponent>(entity);
		castComponent.m_Glyph = hitEvent.m_OnHit;
		castComponent.m_Entity = hitEvent.m_Entity;
		castComponent.m_Translate = hitEvent.m_Translate;
		castComponent.m_Direction = hitEvent.m_Direction;
	}

	for (const auto& hitEvent : world.Events<spellcraft::ProjectileHitEvent>())
	{
		if (!hitEvent.m_OnHit.IsValid())
			continue;

		const ecs::Entity entity = world.CreateEntity();
		auto& castComponent = world.AddComponent<spellcraft::CastComponent>(entity);
		castComponent.m_Glyph = hitEvent.m_OnHit;
		castComponent.m_Entity = hitEvent.m_Entity;
		castComponent.m_Translate = hitEvent.m_Translate;
		castComponent.m_Direction = hitEvent.m_Direction;
	}
}
