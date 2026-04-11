#include "EnginePCH.h"
#include "Engine/VisibilitySystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/VisibilityTemplate.h"

void eng::VisibilitySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const eng::VisibilityTemplate>
		::Include<const eng::VisibilityTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& visibilityTemplate = view.ReadRequired<eng::VisibilityTemplate>();
		auto& visibilityComponent = world.AddComponent<eng::VisibilityComponent>(view);
		visibilityComponent.m_IsVisible = visibilityTemplate.m_IsVisible;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::VisibilityTemplate>
		::Include<eng::VisibilityComponent, const eng::VisibilityTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& visibilityTemplate = view.ReadRequired<eng::VisibilityTemplate>();
		auto& visibilityComponent = view.WriteRequired<eng::VisibilityComponent>();
		visibilityComponent.m_IsVisible = visibilityTemplate.m_IsVisible;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::VisibilityTemplate>>())
	{
		world.RemoveComponent<eng::VisibilityComponent>(view);
	}
}
