#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftExplosionSystem.h"

#include "Core/GameTime.h"
#include "Core/Random.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/TransformComponent.h"
#include "Spellcraft/SpellcraftBookComponent.h"
#include "Spellcraft/SpellcraftCastComponent.h"
#include "Spellcraft/SpellcraftExplosionComponent.h"
#include "Spellcraft/SpellcraftExplosionHitEvent.h"

namespace
{
	const str::Guid strFlipbook = GUID("9cf3dbdc769ea09a6e4aa0390e246666");
}

void spellcraft::ExplosionSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const spellcraft::CastComponent>::Include<const spellcraft::CastComponent>>())
	{
		const auto& bookComponent = world.ReadComponent<spellcraft::BookComponent>();
		const auto& castComponent = view.ReadRequired<spellcraft::CastComponent>();
		const spellcraft::Glyph& glyph = bookComponent.m_Glyph.Get(castComponent.m_Glyph);
		if (std::holds_alternative<spellcraft::emitter::Explosion>(glyph))
		{
			const auto& explosionData = std::get<spellcraft::emitter::Explosion>(glyph);
			auto& explosionComponent = world.AddComponent<spellcraft::ExplosionComponent>(view);
			explosionComponent.m_OnHit = explosionData.m_OnHit;
			explosionComponent.m_Duration = 2.f;
			explosionComponent.m_Radius = explosionData.m_Radius;
			explosionComponent.m_Speed = explosionData.m_Speed;

			auto& transformComponent = world.AddComponent<eng::TransformComponent>(view);
			transformComponent.m_Translate = castComponent.m_Translate;

			auto& spriteComponent = world.AddComponent<eng::FlipbookComponent>(view);
			spriteComponent.m_Flipbook = strFlipbook;
			spriteComponent.m_Size = Vector2u(100, 100);
		}
	}

	for (auto&& view : world.Query<ecs::query::Include<spellcraft::ExplosionComponent, const eng::TransformComponent>>())
	{
		auto& explosionComponent = view.WriteRequired<spellcraft::ExplosionComponent>();
		explosionComponent.m_Duration -= gameTime.m_DeltaTime;

		if (explosionComponent.m_Duration <= 0.f)
		{
			const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();
			auto& hitEvent = world.AddEvent<spellcraft::ExplosionHitEvent>();
			hitEvent.m_Entity = {};
			hitEvent.m_OnHit = explosionComponent.m_OnHit;
			hitEvent.m_Translate = transformComponent.m_Translate;
			hitEvent.m_Direction.x = random::Range(-1.f, +1.f);
			hitEvent.m_Direction.y = random::Range(-1.f, +1.f);
			hitEvent.m_Direction.Normalize();

			world.DestroyEntity(view);
		}
	}
}
