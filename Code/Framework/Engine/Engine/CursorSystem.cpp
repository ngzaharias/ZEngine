#include "EnginePCH.h"
#include "Engine/CursorSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CursorComponent.h"
#include "Engine/InputManager.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"

void eng::CursorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& inputManager = world.ReadResource<eng::InputManager>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, const eng::CursorComponent>>())
	{
		auto& transformComponent = world.AddComponent<eng::TransformComponent>(entity);
		transformComponent.m_Translate;
		transformComponent.m_Rotate;
	}
}