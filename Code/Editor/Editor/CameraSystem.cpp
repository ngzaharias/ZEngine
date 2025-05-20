#include "EditorPCH.h"
#include "Editor/CameraSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/LevelComponents.h"
#include "Engine/TransformComponent.h"

void editor::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const Set<ecs::Entity> entities = world.Query<ecs::query::Include<const eng::camera::EditorComponent>>();
	if (world.HasAny<ecs::query::Added<const eng::level::LoadedComponent>>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
		world.AddComponent<eng::camera::EditorComponent>(entity);
		world.AddComponent<eng::camera::Move3DComponent>(entity);
		world.AddComponent<eng::camera::ProjectionComponent>(entity);
		auto& transform = world.AddComponent<eng::TransformComponent>(entity);
		transform.m_Rotate = Rotator(+45.f, -45.f, 0.f);
		transform.m_Translate = Vector3f(+500.f, +500.f, -500.f);
	}

	if (world.HasAny<ecs::query::Removed<const eng::level::LoadedComponent>>())
	{
		for (const ecs::Entity& entity : entities)
			world.DestroyEntity(entity);
	}
}