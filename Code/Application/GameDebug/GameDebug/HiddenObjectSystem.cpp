#include "GameDebugPCH.h"
#include "GameDebug/HiddenObjectSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "GameDebug/HiddenObjectComponent.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Math/AABB.h"

void debug::hidden::ObjectSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<::hidden::settings::DebugSingleton>();
	if (settings.m_IsObjectEnabled)
	{
		using AddQuery = ecs::query
			::Include<
			const ::hidden::ObjectComponent, 
			const eng::level::EntityComponent, 
			const eng::SpriteComponent, 
			const eng::TransformComponent>
			::Exclude<
			const debug::hidden::ObjectComponent>;
		for (auto&& view : world.Query<AddQuery>())
		{
			auto& parentDebug = world.AddComponent<debug::hidden::ObjectComponent>(view);
			parentDebug.m_Child = world.CreateEntity();

			auto& childLevel = world.AddComponent<eng::level::EntityComponent>(parentDebug.m_Child);
			childLevel = view.ReadRequired<eng::level::EntityComponent>();

			auto& childSprite = world.AddComponent<eng::SpriteComponent>(parentDebug.m_Child);
			childSprite = view.ReadRequired<eng::SpriteComponent>();

			auto& childTransform = world.AddComponent<eng::TransformComponent>(parentDebug.m_Child);
			childTransform = view.ReadRequired<eng::TransformComponent>();

			const auto& parentObject = view.ReadRequired<::hidden::ObjectComponent>();
			childSprite.m_Colour = Colour::Magenta;
		}

		using SpriteQuery = ecs::query
			::Updated<
			const eng::SpriteComponent>
			::Include<
			const ::hidden::ObjectComponent, 
			const debug::hidden::ObjectComponent,
			const eng::SpriteComponent>;
		for (auto&& view : world.Query<SpriteQuery>())
		{
			const auto& parentDebug = view.ReadRequired<debug::hidden::ObjectComponent>();
			const auto& parentObject = view.ReadRequired<::hidden::ObjectComponent>();
			const auto& parentSprite = view.ReadRequired<eng::SpriteComponent>();

			auto& childSprite = world.WriteComponent<eng::SpriteComponent>(parentDebug.m_Child);
			childSprite.m_Colour = Colour::Magenta;
		}

		using TransformQuery = ecs::query
			::Updated<
			eng::TransformComponent>
			::Include<
			const eng::TransformComponent,
			const debug::hidden::ObjectComponent>;
		for (auto&& view : world.Query<TransformQuery>())
		{
			const auto& parentDebug = view.ReadRequired<debug::hidden::ObjectComponent>();
			const auto& parentTransform = view.ReadRequired<eng::TransformComponent>();

			auto& childTransform = world.WriteComponent<eng::TransformComponent>(parentDebug.m_Child);
			childTransform.m_Translate = parentTransform.m_Translate - Vector3f(0.f, 0.f, 1.f);
			childTransform.m_Rotate = parentTransform.m_Rotate;
			childTransform.m_Scale = parentTransform.m_Scale;
		}
	}
	else
	{
		using Query = ecs::query
			::Include<const ::hidden::ObjectComponent, const debug::hidden::ObjectComponent>;
		for (auto&& view : world.Query<Query>())
		{
			const auto& object = view.ReadRequired<debug::hidden::ObjectComponent>();
			world.RemoveComponent<debug::hidden::ObjectComponent>(view);
			world.DestroyEntity(object.m_Child);
		}
	}
}