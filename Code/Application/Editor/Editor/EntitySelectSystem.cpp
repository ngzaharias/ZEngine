#include "EditorPCH.h"
#include "Editor/EntitySelectSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EntitySelectComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/LevelComponents.h"
#include "Engine/LinesComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/SpriteComponent.h"
#include "Engine/SpriteAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/AABB.h"
#include "Math/CollisionMath.h"
#include "Math/Quaternion.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"

namespace
{
	const str::Name strInput = str::Name::Create("EntitySelect");
	const str::Name strSelect = str::Name::Create("EntitySelect_Select");

	struct Selection
	{
		ecs::Entity m_Entity = {};
		float m_DistanceSqr = FLT_MAX;
	};

	Optional<AABB3f> GetEntityBounds(editor::EntitySelectSystem::World& world, const ecs::Entity& entity)
	{
		if (!world.HasComponent<eng::TransformComponent>(entity))
			return {};

		if (world.HasComponent<eng::VisibilityComponent>(entity))
		{
			const auto& visibileComponent = world.ReadComponent<eng::VisibilityComponent>(entity);
			if (!visibileComponent.m_IsVisible)
				return {};
		}

		const auto& entityTransform = world.ReadComponent<eng::TransformComponent>(entity);
		if (world.HasComponent<eng::SpriteComponent>(entity))
		{
			const auto& assetManager = world.ReadResource<eng::AssetManager>();
			const auto& spriteComponent = world.ReadComponent<eng::SpriteComponent>(entity);
			const auto* spriteAsset = assetManager.ReadAsset<eng::SpriteAsset>(spriteComponent.m_Sprite);
			if (!spriteAsset)
				return {};

			const Vector2u spriteSize = Vector2u((uint32)spriteAsset->m_Size.x, (uint32)spriteAsset->m_Size.y);
			const Vector2u componentSize = spriteComponent.m_Size.value_or(spriteSize);

			Vector3f size;
			size.x = componentSize.x * 0.5f;
			size.y = componentSize.y * 0.5f;
			size.z = 0.1f;
			return AABB3f::FromExtents(entityTransform.m_Translate, size);
		}

		return {};
	}
}

void editor::EntitySelectSystem::Initialise(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, input::Layer{ eng::EInputPriority::EditorWorld });
}

void editor::EntitySelectSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void editor::EntitySelectSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<eng::settings::DebugSingleton>())
	{
		const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
		auto& input = world.WriteResource<eng::InputManager>();
		input::Layer& layer = input.ModifyLayer(strInput);
		layer.m_Bindings.RemoveAll();

		if (debugSettings.m_IsEditorModeEnabled)
			layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_1, false);
	}


	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;


	ecs::Entity cameraEntity = {};
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::EditorComponent>>())
		cameraEntity = entity;

	if (!cameraEntity.IsUnassigned())
	{
		const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const auto& input = world.ReadResource<eng::InputManager>();
		const Ray3f ray = eng::camera::ScreenToRay(
			cameraProjection,
			cameraTransform,
			*window,
			input.m_MousePosition);

		Array<Selection> selected = {};
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::EntityComponent, const eng::TransformComponent>>())
		{
			const Optional<AABB3f> aabb = GetEntityBounds(world, entity);
			if (!aabb)
				continue;

			Vector3f intersectPos;
			if (!math::Intersection(ray, *aabb, intersectPos))
				continue;

			selected.Emplace(entity, math::DistanceSqr(ray.m_Position, intersectPos));

			auto& lines = world.WriteSingleton<eng::LinesSingleton>();
			lines.AddSphere(intersectPos, 100.f, Colour::Red);
		}

		if (input.IsPressed(strSelect))
		{
			auto& selectComponent = world.WriteSingleton<editor::EntitySelectSingleton>();
			if (!selected.IsEmpty())
			{
				std::sort(selected.begin(), selected.end(),
					[](const Selection& a, const Selection& b) -> int
					{
						return a.m_DistanceSqr < b.m_DistanceSqr;
					});


				int32 index = 0;

				// #todo: 
				//if (!selectComponent.m_Entity.IsUnassigned())
				//{
				//	const auto itr = std::find_if(selected.begin(), selected.end(),
				//		[&](const auto& lhs)
				//		{
				//			return lhs.m_Entity == selectComponent.m_Entity;
				//		});

				//	if (itr != selected.end())
				//	{
				//		index = (int32)std::distance(selected.begin(), itr);
				//		index = (index + 1) % selected.GetCount();
				//	}
				//}

				const auto& selection = selected[index];
				selectComponent.m_Entity = selection.m_Entity;
			}
			else
			{
				selectComponent.m_Entity = {};
			}
		}
	}
}
