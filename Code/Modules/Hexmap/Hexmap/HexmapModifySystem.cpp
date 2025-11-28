#include "HexmapPCH.h"
#include "Hexmap/HexmapModifySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/InputManager.h"
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
	const str::Name strInput = str::Name::Create("HexmapModify");
	const str::Name strHexmap0 = str::Name::Create("HexmapModify_Hexmap0");
	const str::Name strHexmap1 = str::Name::Create("HexmapModify_Hexmap1");
	const str::Name strHexmap2 = str::Name::Create("HexmapModify_Hexmap2");
	const str::Name strHexmap3 = str::Name::Create("HexmapModify_Hexmap3");
	const str::Name strHexmap4 = str::Name::Create("HexmapModify_Hexmap4");
	const str::Name strHexmap5 = str::Name::Create("HexmapModify_Hexmap5");
	const str::Name strSelect = str::Name::Create("HexmapModify_Select");

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

	if (world.HasAny<ecs::query::Added<hexmap::LayerComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strHexmap0, input::EKey::Numpad_0);
		layer.m_Bindings.Emplace(strHexmap1, input::EKey::Numpad_1);
		layer.m_Bindings.Emplace(strHexmap2, input::EKey::Numpad_2);
		layer.m_Bindings.Emplace(strHexmap3, input::EKey::Numpad_3);
		layer.m_Bindings.Emplace(strHexmap4, input::EKey::Numpad_4);
		layer.m_Bindings.Emplace(strHexmap5, input::EKey::Numpad_5);
		layer.m_Bindings.Emplace(strSelect,  input::EKey::Mouse_Left);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<hexmap::LayerComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	static int32 value = 2;
	const auto& input = world.ReadResource<eng::InputManager>();
	if (input.IsPressed(strHexmap0))
		value = 0;
	if (input.IsPressed(strHexmap1))
		value = 1;
	if (input.IsPressed(strHexmap2))
		value = 2;
	if (input.IsPressed(strHexmap3))
		value = 3;
	if (input.IsPressed(strHexmap4))
		value = 4;
	if (input.IsPressed(strHexmap5))
		value = 5;

	if (input.IsHeld(strSelect))
	{
		using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::ProjectionComponent>;
		for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
		{
			const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Plane3f plane = Plane3f(Vector3f::AxisY, Vector3f::Zero);
			const Ray3f ray = eng::camera::ScreenToRay(
				cameraProjection,
				cameraTransform,
				*window,
				input.m_MousePosition);

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