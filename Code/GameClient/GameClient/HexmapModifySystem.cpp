#include "GameClientPCH.h"
#include "GameClient/HexmapModifySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapChartComponent.h"
#include "GameClient/HexmapFragmentComponent.h"
#include "GameClient/HexmapSettingsComponent.h"
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

	static int32 value = 2;

	const auto& chart = world.ReadSingleton<hexmap::ChartComponent>();
	const auto& settings = world.ReadSingleton<hexmap::SettingsComponent>();

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

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

			const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
			const Matrix4x4 cameraView = transform.ToTransform();

			const Vector3f mousePosition = eng::camera::ScreenToWorld(input.m_MousePosition, camera.m_Projection, cameraView);
			const Vector3f mouseDirection = ToMouseDirection(mousePosition, camera, transform);

			const Ray3f ray = Ray3f(mousePosition, mouseDirection);
			const Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				const Vector2i hexPos = hexagon::ToOffset(intersectPos.XZ(), chart.m_TileRadius);
				for (const ecs::Entity& fragmentEntity : world.Query<ecs::query::Include<eng::TransformComponent, hexmap::FragmentComponent>>())
				{
					const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(fragmentEntity);
					if (fragment.m_Level != chart.m_Level)
						continue;

					const Vector2i min = fragment.m_HexPos;
					const Vector2i max = min + settings.m_TileCount;
					if (hexPos.x < min.x || hexPos.x >= max.x)
						continue;
					if (hexPos.y < min.y || hexPos.y >= max.y)
						continue;

					const Vector2i localPos = hexPos - min;
					const int32 index = math::To1Dimension(localPos, settings.m_TileCount.x);

					auto& write = world.WriteComponent<hexmap::FragmentComponent>(fragmentEntity);
					write.m_Data[index] = value;
				}
			}
		}
	}
}