#include "SpellcraftPCH.h"
#include "SharedSpellcraft/SpellcraftHealthSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedSpellcraft/SpellcraftHealthComponent.h"
#include "SharedSpellcraft/SpellcraftHealthTemplate.h"

void shared::spellcraft::HealthSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const shared::spellcraft::HealthTemplate>
		::Include<const shared::spellcraft::HealthTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& healthTemplate = view.ReadRequired<shared::spellcraft::HealthTemplate>();
		auto& healthComponent = world.AddComponent<shared::spellcraft::HealthComponent>(view);
		healthComponent.m_Current = healthTemplate.m_Initial;
	}

	using UpdatedQuery = ecs::query
		::Updated<const shared::spellcraft::HealthTemplate>
		::Include<shared::spellcraft::HealthComponent, const shared::spellcraft::HealthTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& healthTemplate = view.ReadRequired<shared::spellcraft::HealthTemplate>();
		auto& healthComponent = view.WriteRequired<shared::spellcraft::HealthComponent>();
		healthComponent.m_Current = healthTemplate.m_Initial;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const shared::spellcraft::HealthTemplate>>())
	{
		world.RemoveComponent<shared::spellcraft::HealthComponent>(view);
	}
}
