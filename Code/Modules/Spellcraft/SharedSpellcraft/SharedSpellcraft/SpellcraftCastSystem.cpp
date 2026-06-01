#include "SpellcraftPCH.h"
#include "SharedSpellcraft/SpellcraftCastSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedSpellcraft/SpellcraftBookComponent.h"
#include "SharedSpellcraft/SpellcraftCastComponent.h"
#include "SharedSpellcraft/SpellcraftExplosionComponent.h"
#include "SharedSpellcraft/SpellcraftExplosionHitEvent.h"
#include "SharedSpellcraft/SpellcraftProjectileComponent.h"
#include "SharedSpellcraft/SpellcraftProjectileHitEvent.h"

void shared::spellcraft::CastSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& hitEvent : world.Events<const shared::spellcraft::ExplosionHitEvent>())
	{
		if (!hitEvent.m_OnHit.IsValid())
			continue;

		const ecs::Entity entity = world.CreateEntity();
		auto& castComponent = world.AddComponent<shared::spellcraft::CastComponent>(entity);
		castComponent.m_Glyph = hitEvent.m_OnHit;
		castComponent.m_Entity = hitEvent.m_Entity;
		castComponent.m_Translate = hitEvent.m_Translate;
		castComponent.m_Direction = hitEvent.m_Direction;
	}

	for (const auto& hitEvent : world.Events<const shared::spellcraft::ProjectileHitEvent>())
	{
		if (!hitEvent.m_OnHit.IsValid())
			continue;

		const ecs::Entity entity = world.CreateEntity();
		auto& castComponent = world.AddComponent<shared::spellcraft::CastComponent>(entity);
		castComponent.m_Glyph = hitEvent.m_OnHit;
		castComponent.m_Entity = hitEvent.m_Entity;
		castComponent.m_Translate = hitEvent.m_Translate;
		castComponent.m_Direction = hitEvent.m_Direction;
	}
}
