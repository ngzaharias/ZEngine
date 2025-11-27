#include "EditorPCH.h"
#include "Editor/CameraSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void editor::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	const bool hasEditorCamera = world.HasAny<ecs::query::Include<const eng::camera::EditorComponent>>();
	if (!hasEditorCamera && debugSettings.m_IsEditorModeEnabled)
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
		world.AddComponent<eng::camera::EditorComponent>(entity);
		world.AddComponent<eng::camera::Move3DComponent>(entity);
		world.AddComponent<eng::camera::Pan3DComponent>(entity);
		world.AddComponent<eng::camera::ProjectionComponent>(entity);
		world.AddComponent<eng::camera::Zoom2DComponent>(entity);
		auto& transform = world.AddComponent<eng::TransformComponent>(entity);
		transform.m_Rotate = Rotator(+20.f, -45.f, 0.f);
		transform.m_Translate = Vector3f(+1000.f, +500.f, -1000.f);
	}
	else if (hasEditorCamera && !debugSettings.m_IsEditorModeEnabled)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::EditorComponent>>())
			world.DestroyEntity(entity);
	}
}