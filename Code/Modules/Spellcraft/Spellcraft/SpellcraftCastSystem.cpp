#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftCastSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Spellcraft/SpellcraftBookComponent.h"
#include "Spellcraft/SpellcraftCastComponent.h"
#include "Spellcraft/SpellcraftExplosionComponent.h"
#include "Spellcraft/SpellcraftExplosionHitEvent.h"
#include "Spellcraft/SpellcraftProjectileComponent.h"
#include "Spellcraft/SpellcraftProjectileHitEvent.h"

namespace
{
	const str::Name strCast = NAME("Cast");
	const str::Name strInput = NAME("spellcraft::CastSystem");
	const str::Guid strProjectile = GUID("e1967994f1424aa4afc4f1ee1743a16a");
}

void spellcraft::CastSystem::Initialise(World& world)
{
	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::Gameplay;
	layer.m_Bindings.Emplace(strCast, input::EKey::Mouse_1, true);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, layer);
}

void spellcraft::CastSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& input = world.ReadResource<eng::InputManager>();
	if (input.IsPressed(strCast))
	{
		const ecs::Entity entity = world.CreateEntity();
		auto& castComponent = world.AddComponent<spellcraft::CastComponent>(entity);
		castComponent.m_Glyph = strProjectile;
		castComponent.m_Direction = Vector3f::AxisX;
		castComponent.m_Translate = Vector3f::Zero;
	}

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
