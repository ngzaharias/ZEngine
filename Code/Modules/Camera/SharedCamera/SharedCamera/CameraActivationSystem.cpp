#include "CameraPCH.h"
#include "SharedCamera/CameraActivationSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/SettingsDebugComponent.h"
#include "Engine/TransformComponent.h"
#include "SharedCamera/CameraMove3DComponent.h"
#include "SharedCamera/CameraPan3DComponent.h"
#include "SharedCamera/CameraZoom2DComponent.h"
#include "SharedGameState/GameStateEditModeComponent.h"

void shared::camera::ActivationSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_EDITOR
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
	world.AddComponent<eng::CameraComponent>(entity);
	world.AddComponent<eng::EditorComponent>(entity);
	world.AddComponent<shared::camera::Move3DComponent>(entity);
	world.AddComponent<shared::camera::Pan3DComponent>(entity);
	world.AddComponent<shared::camera::Zoom2DComponent>(entity);

	auto& transform = world.AddComponent<eng::TransformComponent>(entity);
	transform.m_Rotate = Rotator(+20.f, -45.f, 0.f);
	transform.m_Translate = Vector3f(+1000.f, +500.f, -1000.f);
#endif
}

void shared::camera::ActivationSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Include<shared::gamestate::EditModeComponent>>())
	{
		using EnableQuery = ecs::query
			::Include<
			const eng::CameraComponent, 
			const eng::EditorComponent>
			::Exclude<
			const eng::ActiveComponent>;
		for (auto&& view : world.Query<EnableQuery>())
		{
			world.AddComponent<eng::ActiveComponent>(view);
		}

		using DisableQuery = ecs::query
			::Include<
			const eng::ActiveComponent, 
			const eng::CameraComponent>
			::Exclude<
			const eng::EditorComponent>;
		for (auto&& view : world.Query<DisableQuery>())
		{
			world.RemoveComponent<eng::ActiveComponent>(view);
		}
	}
	else
	{
		using EnableQuery = ecs::query
			::Include<
			const eng::CameraComponent>
			::Exclude<
			const eng::ActiveComponent, 
			const eng::EditorComponent>;
		for (auto&& view : world.Query<EnableQuery>())
		{
			world.AddComponent<eng::ActiveComponent>(view);
		}

		using DisableQuery = ecs::query
			::Include<
			const eng::ActiveComponent,
			const eng::CameraComponent,
			const eng::EditorComponent>;
		for (auto&& view : world.Query<DisableQuery>())
		{
			world.RemoveComponent<eng::ActiveComponent>(view);
		}
	}
}