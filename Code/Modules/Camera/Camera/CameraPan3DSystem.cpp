#include "CameraPCH.h"
#include "Camera/CameraPan3DSystem.h"

#include "Camera/CameraPan3DComponent.h"
#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
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
	const str::Name strInput = str::Name::Create("CameraPan3D");
	const str::Name strSelect = str::Name::Create("CameraPan3D_Select");
}

void camera::Pan3DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasComponent = world.HasAny<ecs::query::Include<camera::Pan3DComponent>>();

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
		const camera::Pan3DComponent, 
		const eng::CameraComponent>;

	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		const auto& camera = view.ReadRequired<eng::CameraComponent>();
		const auto& transform = view.ReadRequired<eng::TransformComponent>();

		if (input.IsHeld(strSelect))
		{
			const Vector3f worldPosA = eng::ScreenToWorld(
				camera.m_Projection,
				Matrix4x4::Identity,
				windowSize,
				Vector2f::Zero);
			const Vector3f worldPosB = eng::ScreenToWorld(
				camera.m_Projection,
				Matrix4x4::Identity,
				windowSize,
				input.m_MouseDelta);

			const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
			const Vector3f delta = (worldPosB - worldPosA) * rotation;
			const Vector3f translate = transform.m_Translate + delta;

			if (!math::IsNearly(transform.m_Translate, translate))
			{
				auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
				writeTransform.m_Translate = translate;
			}
		}
	}
}