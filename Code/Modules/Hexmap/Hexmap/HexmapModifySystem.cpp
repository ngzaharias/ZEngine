#include "HexmapPCH.h"
#include "Hexmap/HexmapModifySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Hexmap/HexmapHelpers.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Math/CollisionMath.h"
#include "Math/Hexagon.h"
#include "Math/HexagonHelpers.h"
#include "Math/Plane.h"
#include "Math/Ray.h"

namespace
{
	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
			return Vector3f::AxisZ * cameraRotate;
		}

		return (mousePosition - transform.m_Translate).Normalized();
	}
}

void hexmap::ModifySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	static int32 value = 2;

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
	{
		const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

		if (input.IsKeyPressed(input::EKeyboard::Num_0))
			value = 0;
		if (input.IsKeyPressed(input::EKeyboard::Num_1))
			value = 1;
		if (input.IsKeyPressed(input::EKeyboard::Num_2))
			value = 2;
		if (input.IsKeyPressed(input::EKeyboard::Num_3))
			value = 3;
		if (input.IsKeyPressed(input::EKeyboard::Num_4))
			value = 4;
		if (input.IsKeyPressed(input::EKeyboard::Num_5))
			value = 5;


		if (!input.IsKeyHeld(input::EMouse::Left))
			continue;

		for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
		{
			const auto& camera = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Matrix4x4 cameraView = transform.ToTransform();
			const Vector3f mousePosition = eng::camera::ScreenToWorld(input.m_MousePosition, camera.m_Projection, cameraView, resolution);
			const Vector3f mouseDirection = ToMouseDirection(mousePosition, camera, transform);

			const Ray3f ray = Ray3f(mousePosition, mouseDirection);
			const Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				for (const ecs::Entity& layerEntity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::LayerComponent>>())
				{
					const auto& layer = world.ReadComponent<hexmap::LayerComponent>(layerEntity);
					const auto& root = world.ReadComponent<hexmap::RootComponent>(layer.m_Root);

					const HexPos hexPos = hexagon::ToOffset(intersectPos.XZ(), root.m_HexRadius);
					const HexPos min = ToHexPos(layer.m_Origin, layer.m_HexCount);
					const HexPos max = min + layer.m_HexCount;
					if (hexPos.x < min.x || hexPos.x >= max.x)
						continue;
					if (hexPos.y < min.y || hexPos.y >= max.y)
						continue;

					const Vector2i localPos = hexPos - min;
					const int32 index = math::To1Dimension(localPos, root.m_HexCount.x);

					auto& write = world.WriteComponent<hexmap::LayerComponent>(layerEntity);
					write.m_HexData[index] = { value };
				}
			}
		}
	}
}