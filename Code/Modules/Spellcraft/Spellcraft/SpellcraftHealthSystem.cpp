#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftHealthSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Spellcraft/SpellcraftHealthComponent.h"
#include "Spellcraft/SpellcraftHealthTemplate.h"

void spellcraft::HealthSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const spellcraft::HealthTemplate>
		::Include<const spellcraft::HealthTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& healthTemplate = view.ReadRequired<spellcraft::HealthTemplate>();
		auto& healthComponent = world.AddComponent<spellcraft::HealthComponent>(view);
		healthComponent.m_Current = healthTemplate.m_Initial;
	}

	using UpdatedQuery = ecs::query
		::Updated<const spellcraft::HealthTemplate>
		::Include<spellcraft::HealthComponent, const spellcraft::HealthTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& healthTemplate = view.ReadRequired<spellcraft::HealthTemplate>();
		auto& healthComponent = view.WriteRequired<spellcraft::HealthComponent>();
		healthComponent.m_Current = healthTemplate.m_Initial;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const spellcraft::HealthTemplate>>())
	{
		world.RemoveComponent<spellcraft::HealthComponent>(view);
	}
}
