#include "EnginePCH.h"
#include "Engine/StaticMeshSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/StaticMeshTemplate.h"

void eng::StaticMeshSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const eng::StaticMeshTemplate>
		::Include<const eng::StaticMeshTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& meshTemplate = view.ReadRequired<eng::StaticMeshTemplate>();
		auto& meshComponent = world.AddComponent<eng::StaticMeshComponent>(view);
		meshComponent.m_StaticMesh = meshTemplate.m_StaticMesh;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::StaticMeshTemplate>
		::Include<eng::StaticMeshComponent, const eng::StaticMeshTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& meshTemplate = view.ReadRequired<eng::StaticMeshTemplate>();
		auto& meshComponent = view.WriteRequired<eng::StaticMeshComponent>();
		meshComponent.m_StaticMesh = meshTemplate.m_StaticMesh;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::StaticMeshTemplate>>())
	{
		world.RemoveComponent<eng::StaticMeshComponent>(view);
	}
}
