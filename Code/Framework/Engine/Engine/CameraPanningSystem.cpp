#include "EnginePCH.h"
#include "Engine/CameraPanningSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraPan3DComponent.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/InputManager.h"
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

	const bool hasComponent = world.HasAny<ecs::query::Include<eng::camera::Pan3DComponent>>();

	auto& input = world.WriteResource<eng::InputManager>();
	if (hasComponent && !input.HasLayer(strInput))
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);
		input.AppendLayer(strInput, layer);
	}

	if (!hasComponent && input.HasLayer(strInput))
	{
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();

	using CameraQuery = ecs::query
		::Include<
		eng::TransformComponent, 
		const eng::camera::Pan3DComponent, 
		const eng::camera::ProjectionComponent>;
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& projection = view.ReadRequired<eng::camera::ProjectionComponent>();

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsHeld(strSelect))
		{
			const Vector3f worldPosA = eng::camera::ScreenToWorld(
				projection.m_Projection,
				Matrix4x4::Identity,
				windowSize,
				Vector2f::Zero);
			const Vector3f worldPosB = eng::camera::ScreenToWorld(
				projection.m_Projection,
				Matrix4x4::Identity,
				windowSize,
				input.m_MouseDelta);

			auto& transform = view.WriteRequired<eng::TransformComponent>();
			const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
			const Vector3f delta = (worldPosB - worldPosA) * rotation;
			transform.m_Translate += delta;
		}
	}
}