#include "EnginePCH.h"
#include "Engine/TransformSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/TransformComponent.h"
#include "Engine/TransformTemplate.h"

void eng::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const eng::TransformTemplate>
		::Include<const eng::TransformTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& transformTemplate = view.ReadRequired<eng::TransformTemplate>();
		auto& transformComponent = world.AddComponent<eng::TransformComponent>(view);
		transformComponent.m_Translate = transformTemplate.m_Translate;
		transformComponent.m_Rotate = transformTemplate.m_Rotate;
		transformComponent.m_Scale = transformTemplate.m_Scale;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::TransformTemplate>
		::Include<eng::TransformComponent, const eng::TransformTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& transformTemplate = view.ReadRequired<eng::TransformTemplate>();
		auto& transformComponent = view.WriteRequired<eng::TransformComponent>();
		transformComponent.m_Translate = transformTemplate.m_Translate;
		transformComponent.m_Rotate = transformTemplate.m_Rotate;
		transformComponent.m_Scale = transformTemplate.m_Scale;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::TransformTemplate>>())
	{
		world.RemoveComponent<eng::TransformComponent>(view);
	}
}