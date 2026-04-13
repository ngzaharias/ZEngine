#include "EnginePCH.h"
#include "Engine/CameraSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraTemplate.h"

void eng::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<const eng::CameraTemplate>
		::Include<const eng::CameraTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& cameraTemplate = view.ReadRequired<eng::CameraTemplate>();
		auto& cameraComponent = world.AddComponent<eng::CameraComponent>(view);
		cameraComponent.m_Projection = cameraTemplate.m_Projection;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::CameraTemplate>
		::Include<eng::CameraComponent, const eng::CameraTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& cameraTemplate = view.ReadRequired<eng::CameraTemplate>();
		auto& cameraComponent = view.WriteRequired<eng::CameraComponent>();
		cameraComponent.m_Projection = cameraTemplate.m_Projection;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::CameraTemplate>>())
	{
		world.RemoveComponent<eng::CameraComponent>(view);
	}
}