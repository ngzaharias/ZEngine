#include "GameClientPCH.h"
#include "GameClient/HexmapRootSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapRootComponent.h"
#include "Math/AABB.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"
#include "Math/Plane.h"
#include "Math/Vector.h"

namespace
{
	AABB2f GetCameraZone(const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		constexpr Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);

		const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
		const Vector3f cameraForward = Vector3f::AxisZ * cameraRotate;

		const Vector2f screenCentre = Vector2f(Screen::width, Screen::height) * 0.5f;
		const Vector2f pixelPosA = screenCentre - screenCentre;
		const Vector2f pixelPosB = screenCentre + screenCentre;

		const Matrix4x4 cameraView = Matrix4x4::FromTransform(transform.m_Translate, Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f)), 1.f);
		const Vector3f worldPosA = eng::camera::ScreenToWorld(pixelPosA, camera.m_Projection, cameraView);
		const Vector3f worldPosB = eng::camera::ScreenToWorld(pixelPosB, camera.m_Projection, cameraView);

		Vector3f forwardA = cameraForward;
		Vector3f forwardB = cameraForward;
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

	bool hasChanged = false;
	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<eng::InputComponent>>())
	{
		const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);
		if (input.m_ScrollDelta.y == 0)
			continue;

		for (const ecs::Entity& rootEntity : world.Query<ecs::query::Include<hexmap::RootComponent>>())
		{
			auto& root = world.WriteComponent<hexmap::RootComponent>(rootEntity);
			root.m_Zoom -= input.m_ScrollDelta.y * 0.1f;
			if (root.m_Zoom > 1.f)
			{
				root.m_Depth++;
				root.m_Zoom = 0.f;
			}
			if (root.m_Zoom < 0.f)
			{
				root.m_Depth--;
				root.m_Zoom = 1.f;
			}
		}
	}

	const bool cameraAdded = world.HasAny<ecs::query::Added<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool cameraChanged = world.HasAny<ecs::query::Updated<eng::camera::ProjectionComponent>::Include<eng::TransformComponent>>();
	const bool transformAdded = world.HasAny<ecs::query::Added<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	const bool transformChanged = world.HasAny<ecs::query::Updated<eng::TransformComponent>::Include<eng::camera::ProjectionComponent>>();
	if (hasChanged || cameraAdded || cameraChanged || transformAdded || transformChanged)
	{
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<eng::camera::ProjectionComponent, eng::TransformComponent>>())
		{
			for (const ecs::Entity& rootEntity : world.Query<ecs::query::Include<hexmap::RootComponent>>())
			{
				const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
				auto& camera = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
				auto& root = world.WriteComponent<hexmap::RootComponent>(rootEntity);

				if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
				{
					constexpr float zoomMin = 1000.f;
					auto& projection = std::get<eng::camera::Orthographic>(camera.m_Projection);
					projection.m_Size = math::Lerp(zoomMin, zoomMin * root.m_HexRatio, root.m_Zoom);
				}

				root.m_Zone = GetCameraZone(camera, transform);
			}
		}
	}
}