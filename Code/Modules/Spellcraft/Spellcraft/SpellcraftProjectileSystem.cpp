#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftProjectileSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Spellcraft/SpellcraftBookComponent.h"
#include "Spellcraft/SpellcraftCastComponent.h"
#include "Spellcraft/SpellcraftProjectileComponent.h"
#include "Spellcraft/SpellcraftProjectileHitEvent.h"

namespace
{
	const str::Guid strSprite = GUID("52ffdca6bc1d64230eda0e2056e9662b");
}

void spellcraft::ProjectileSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const spellcraft::CastComponent>::Include<const spellcraft::CastComponent>>())
	{
		const auto& bookComponent = world.ReadComponent<spellcraft::BookComponent>();
		const auto& castComponent = view.ReadRequired<spellcraft::CastComponent>();
		const spellcraft::Glyph& glyph = bookComponent.m_Glyph.Get(castComponent.m_Glyph);
		if (std::holds_alternative<spellcraft::emitter::Projectile>(glyph))
		{
			const auto& projectileData = std::get<spellcraft::emitter::Projectile>(glyph);
			auto& projectileComponent = world.AddComponent<spellcraft::ProjectileComponent>(view);
			projectileComponent.m_OnHit = projectileData.m_OnHit;
			projectileComponent.m_Direction = castComponent.m_Direction;
			projectileComponent.m_Distance = projectileData.m_Distance;
			projectileComponent.m_Speed = projectileData.m_Speed;

			auto& transformComponent = world.AddComponent<eng::TransformComponent>(view);
			transformComponent.m_Translate = castComponent.m_Translate;

			auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(view);
			spriteComponent.m_Sprite = strSprite;
			spriteComponent.m_Size = Vector2u(100, 100);
		}
	}

	for (auto&& view : world.Query<ecs::query::Include<eng::TransformComponent, spellcraft::ProjectileComponent>>())
	{
		auto& projectileComponent = view.WriteRequired<spellcraft::ProjectileComponent>();
		projectileComponent.m_Distance -= projectileComponent.m_Speed * gameTime.m_DeltaTime;

		auto& transformComponent = view.WriteRequired<eng::TransformComponent>();
		transformComponent.m_Translate += projectileComponent.m_Direction * projectileComponent.m_Speed * gameTime.m_DeltaTime;

		if (projectileComponent.m_Distance <= 0.f)
		{
			auto& hitEvent = world.AddEvent<spellcraft::ProjectileHitEvent>();
			hitEvent.m_Entity = {};
			hitEvent.m_OnHit = projectileComponent.m_OnHit;
			hitEvent.m_Translate = transformComponent.m_Translate;
			hitEvent.m_Direction = projectileComponent.m_Direction;

			world.DestroyEntity(view);
		}
	}
}
