#include "GameDebugPCH.h"
#include "GameDebug/HiddenObjectSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/LinesComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameDebug/HiddenObjectComponent.h"
#include "Math/AABB.h"

namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void dbg::hidden::ObjectSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (settings.m_AreHiddenEnabled)
	{
		using AddQuery = ecs::query
			::Include<
			const ::hidden::ObjectComponent, 
			const eng::level::EntityComponent, 
			const eng::TransformComponent>
			::Exclude<const dbg::hidden::ObjectComponent>;
		for (const ecs::Entity& entity : world.Query<AddQuery>())
		{
			const auto& parentLevel = world.ReadComponent<eng::level::EntityComponent>(entity);

			auto& parentDebug = world.AddComponent<dbg::hidden::ObjectComponent>(entity);
			parentDebug.m_Child = world.CreateEntity();

			world.AddComponent<eng::level::EntityComponent>(parentDebug.m_Child, parentLevel.m_Name);
			world.AddComponent<eng::SpriteComponent>(parentDebug.m_Child);
			world.AddComponent<eng::TransformComponent>(parentDebug.m_Child);
		}

		using UpdateQuery = ecs::query
			::Include<
			const ::hidden::ObjectComponent,
			const dbg::hidden::ObjectComponent,
			const eng::SpriteComponent,
			const eng::TransformComponent>;
		for (const ecs::Entity& entity : world.Query<UpdateQuery>())
		{
			const auto& parentDebug = world.ReadComponent<dbg::hidden::ObjectComponent>(entity);
			const auto& parentObject = world.ReadComponent<::hidden::ObjectComponent>(entity);
			const auto& parentSprite = world.ReadComponent<eng::SpriteComponent>(entity);
			const auto& parentTransform = world.ReadComponent<eng::TransformComponent>(entity);

			auto& childSprite = world.WriteComponent<eng::SpriteComponent>(parentDebug.m_Child);
			childSprite = parentSprite;
			for (const ::hidden::Effect& effect : parentObject.m_Effects)
			{
				core::VariantMatch(effect,
					[&](const ::hidden::SetColour& data)
					{
						childSprite.m_Colour = data.m_Colour;
					},
					[&](const ::hidden::SetSprite& data)
					{
						childSprite.m_Sprite = data.m_Sprite;
					});
			}

			auto& childTransform = world.WriteComponent<eng::TransformComponent>(parentDebug.m_Child);
			childTransform.m_Translate = parentTransform.m_Translate - Vector3f(0.f, 0.f, 1.f);
			childTransform.m_Rotate = parentTransform.m_Rotate;
			childTransform.m_Scale = parentTransform.m_Scale;
		}
	}
	else
	{
		using Query = ecs::query
			::Include<const ::hidden::ObjectComponent, const dbg::hidden::ObjectComponent>;
		for (const ecs::Entity& entity : world.Query<Query>())
		{
			const auto& object = world.ReadComponent<dbg::hidden::ObjectComponent>(entity);
			world.RemoveComponent<dbg::hidden::ObjectComponent>(entity);
			world.DestroyEntity(object.m_Child);
		}
	}
}