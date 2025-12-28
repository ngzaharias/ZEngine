#include "EditorPCH.h"
#include "Editor/CameraSystem.h"

#include "Camera/CameraMove3DComponent.h"
#include "Camera/CameraPan3DComponent.h"
#include "Camera/CameraZoom2DComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void editor::CameraSystem::Initialise(World& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
	world.AddComponent<camera::Move3DComponent>(entity);
	world.AddComponent<camera::Pan3DComponent>(entity);
	world.AddComponent<camera::Zoom2DComponent>(entity);
	world.AddComponent<eng::CameraComponent>(entity);
	world.AddComponent<eng::EditorComponent>(entity);

	auto& transform = world.AddComponent<eng::TransformComponent>(entity);
	transform.m_Rotate = Rotator(+20.f, -45.f, 0.f);
	transform.m_Translate = Vector3f(+1000.f, +500.f, -1000.f);
}

void editor::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	const bool hasEditorCamera = world.HasAny<ecs::query::Include<const eng::EditorComponent>>();
	if (debugSettings.m_IsEditorModeEnabled)
	{
		using EnableQuery = ecs::query
			::Include<const eng::CameraComponent, const eng::EditorComponent>
			::Exclude<const eng::ActiveComponent>;
		for (auto&& view : world.Query<EnableQuery>())
		{
			world.AddComponent<eng::ActiveComponent>(view);
		}

		using DisableQuery = ecs::query
			::Include<const eng::ActiveComponent, const eng::CameraComponent>
			::Exclude<const eng::EditorComponent>;
		for (auto&& view : world.Query<DisableQuery>())
		{
			world.RemoveComponent<eng::ActiveComponent>(view);
		}
	}
	else
	{
		using EnableQuery = ecs::query
			::Include<const eng::CameraComponent>
			::Exclude<const eng::ActiveComponent, const eng::EditorComponent>;
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