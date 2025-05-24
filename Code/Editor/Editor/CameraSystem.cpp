#include "EditorPCH.h"
#include "Editor/CameraSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"

void editor::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (debugSettings.m_IsEditorModeEnabled && !world.HasAny<ecs::query::Include<const eng::camera::EditorComponent>>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
		world.AddComponent<eng::camera::EditorComponent>(entity);
		world.AddComponent<eng::camera::Move3DComponent>(entity);
		world.AddComponent<eng::camera::ProjectionComponent>(entity);
		auto& transform = world.AddComponent<eng::TransformComponent>(entity);
		transform.m_Rotate = Rotator(+20.f, -45.f, 0.f);
		transform.m_Translate = Vector3f(+1000.f, +500.f, -1000.f);
	}

	if (!debugSettings.m_IsEditorModeEnabled && world.HasAny<ecs::query::Include<const eng::camera::EditorComponent>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::EditorComponent>>())
			world.DestroyEntity(entity);
	}
}