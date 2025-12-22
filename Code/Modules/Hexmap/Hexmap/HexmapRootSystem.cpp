#include "HexmapPCH.h"
#include "Hexmap/HexmapRootSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/InputManager.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Math/AABB.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"
#include "Math/Plane.h"
#include "Math/Vector.h"

namespace
{
	const str::Name strInput = str::Name::Create("HexmapRoot");
	const str::Name strSelect = str::Name::Create("HexmapRoot_Select");

	AABB2f GetCameraZone(const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform, const Vector2u& windowSize)
	{
		constexpr Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);

		const Vector2f screenCentre = Vector2f((float)windowSize.x, (float)windowSize.y) * 0.5f;
		const Vector2f pixelPosA = screenCentre - screenCentre;
		const Vector2f pixelPosB = screenCentre + screenCentre;

		const Matrix4x4 cameraView = Matrix4x4::FromTransform(transform.m_Translate, Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f)), 1.f);
		const Vector3f worldPosA = eng::camera::ScreenToWorld(camera.m_Projection, cameraView, windowSize, pixelPosA);
		const Vector3f worldPosB = eng::camera::ScreenToWorld(camera.m_Projection, cameraView, windowSize, pixelPosB);

		Vector3f forwardA = -Vector3f::AxisY;
		Vector3f forwardB = -Vector3f::AxisY;
		if (!std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			forwardA = (worldPosA - transform.m_Translate).Normalized();
			forwardB = (worldPosB - transform.m_Translate).Normalized();
		}

		Vector3f intersectA, intersectB;
		const Ray3f rayA = Ray3f(worldPosA, forwardA);
		const Ray3f rayB = Ray3f(worldPosB, forwardB);
		if (math::Intersection(rayA, plane, intersectA) && math::Intersection(rayB, plane, intersectB))
		{
			AABB2f aabb;
			aabb.m_Min.x = math::Min(intersectA.x, intersectB.x);
			aabb.m_Min.y = math::Min(intersectA.z, intersectB.z);
			aabb.m_Max.x = math::Max(intersectA.x, intersectB.x);
			aabb.m_Max.y = math::Max(intersectA.z, intersectB.z);
			return aabb;
		}
		return {};
	}
}

void hexmap::RootSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<hexmap::RootComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<hexmap::RootComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	bool hasChanged = false;
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.m_ScrollDelta.y != 0)
		{
			for (auto&& view : world.Query<ecs::query::Include<hexmap::RootComponent>>())
			{
				auto& root = view.WriteRequired<hexmap::RootComponent>();
				root.m_Zoom -= input.m_ScrollDelta.y * 0.1f;
				root.m_Zoom = math::Clamp(root.m_Zoom, 0.f, 1.f);
			}
		}
	}

	const Vector2u& windowSize = window->GetSize();
	const bool cameraAdded = world.HasAny<ecs::query::Added<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool cameraChanged = world.HasAny<ecs::query::Updated<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool transformAdded = world.HasAny<ecs::query::Added<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	const bool transformChanged = world.HasAny<ecs::query::Updated<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	if (hasChanged || cameraAdded || cameraChanged || transformAdded || transformChanged)
	{
		for (auto&& cameraView : world.Query<ecs::query::Include<eng::camera::ProjectionComponent, eng::TransformComponent>>())
		{
			const auto& transform = cameraView.ReadRequired<eng::TransformComponent>();
			auto& camera = cameraView.WriteRequired<eng::camera::ProjectionComponent>();

			for (auto&& rootView : world.Query<ecs::query::Include<hexmap::RootComponent>>())
			{
				auto& root = rootView.WriteRequired<hexmap::RootComponent>();

				if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
				{
					constexpr float zoomMin = 1000.f;
					constexpr float zoomMax = 6000.f;

					auto& projection = std::get<eng::camera::Orthographic>(camera.m_Projection);
					projection.m_Size = math::Lerp(zoomMin, zoomMax, root.m_Zoom);
				}

				root.m_Zone = GetCameraZone(camera, transform, windowSize);
			}
		}
	}
}