#include "GameClientPCH.h"
#include "GameClient/HexamapGridSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"
#include "Math/HexagonHelpers.h"
#include "Math/Plane.h"
#include "Math/Ray.h"

void hexamap::GridSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();

	int32 count = 3;
	float sizeA = 500.f;
	float sizeB = sizeA / 5.f;
	float sizeC = sizeB / 5.f;

	for (const Vector2i& gridPos : enumerate::Hexagonal(count))
	{
		const Vector2f worldPos = hexagon::ToWorldPos(gridPos, sizeA);
		linesComponent.AddHexagon(worldPos.XY0(), sizeA, Vector4f(1.f));
	}

	for (const Vector2i& gridPos : enumerate::Hexagonal(count))
	{
		const Vector2f worldPos = hexagon::ToWorldPos(gridPos, sizeB);
		linesComponent.AddHexagon(worldPos.XY0(), sizeB, Vector4f(1.f));
	}

	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent>>())
		{
			const auto& camera = world.GetComponent<const eng::CameraComponent>(cameraEntity);
			const auto& transform = world.GetComponent<const eng::TransformComponent>(cameraEntity);
			const auto& input = world.GetComponent<const eng::InputComponent>(inputEntity);

			Vector3f position = camera::ScreenToWorld(
				input.m_MousePosition,
				camera.m_Projection,
				transform.ToTransform());

			{
				Ray3f ray;
				ray.m_Position = position;
				ray.m_Direction = (position - transform.m_Translate).Normalized();

				Plane3f plane;
				plane.m_Point = Vector3f::Zero;
				plane.m_Normal = Vector3f::AxisZ;

				math::Intersection(ray, plane, position);
			}

			Vector2i gridPosB = hexagon::ToGridPos(position.XY(), sizeB);
			Vector2f worldPosB = hexagon::ToWorldPos(gridPosB, sizeB);

			for (const Vector2i& gridPosC : enumerate::Hexagonal(count))
			{
				const Vector2f worldPosC = worldPosB + hexagon::ToWorldPos(gridPosC, sizeC);
				linesComponent.AddHexagon(worldPosC.XY0(), sizeC, Vector4f(1.f));
			}
		}
	}
}