#include "EnginePCH.h"
#include "Engine/CameraPanningSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

namespace
{
	const str::Name strInput = str::Name::Create("CameraPanning");
	const str::Name strSelect = str::Name::Create("CameraPanning_Select");
}

void eng::camera::PanningSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const int32 count = world.Query<ecs::query::Include<eng::camera::Pan3DComponent>>().GetCount();
	if (count >= 1 && world.HasAny<ecs::query::Added<eng::camera::Pan3DComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (count == 0 && world.HasAny<ecs::query::Removed<eng::camera::Pan3DComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Pan3DComponent, const eng::camera::ProjectionComponent>;

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& projection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsHeld(strSelect))
		{
			const Vector3f worldPosA = eng::camera::ScreenToWorld(
				Vector2f::Zero,
				projection.m_Projection,
				Matrix4x4::Identity,
				resolution);
			const Vector3f worldPosB = eng::camera::ScreenToWorld(
				input.m_MouseDelta,
				projection.m_Projection,
				Matrix4x4::Identity,
				resolution);

			auto& transform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
			const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
			const Vector3f delta = (worldPosB - worldPosA) * rotation;
			transform.m_Translate += delta;
		}
	}
}