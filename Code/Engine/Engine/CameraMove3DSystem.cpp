#include "EnginePCH.h"
#include "Engine/CameraMove3DSystem.h"

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
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/VectorMath.h"

namespace
{
	const str::Guid strInputGuid = str::Guid::Generate();
	const str::Name strMoveBackward = str::Name::Create("CameraMove3D_MoveBackward");
	const str::Name strMoveDown = str::Name::Create("CameraMove3D_MoveDown");
	const str::Name strMoveForward = str::Name::Create("CameraMove3D_MoveForward");
	const str::Name strMoveLeft = str::Name::Create("CameraMove3D_MoveLeft");
	const str::Name strMoveRight = str::Name::Create("CameraMove3D_MoveRight");
	const str::Name strMoveUp = str::Name::Create("CameraMove3D_MoveUp");
	const str::Name strRotate = str::Name::Create("CameraMove3D_Rotate");
	const str::Name strSpeedUpA = str::Name::Create("CameraMove3D_SpeedUpA");
	const str::Name strSpeedUpB = str::Name::Create("CameraMove3D_SpeedUpB");
}

void eng::camera::Move3DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const int32 count = world.Query<ecs::query::Include<eng::camera::Move3DComponent>>().GetCount();
	if (count == 1 && world.HasAny<ecs::query::Added<eng::camera::Move3DComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(input::EKeyboard::A, strMoveLeft);
		layer.m_Bindings.Emplace(input::EKeyboard::D, strMoveRight);
		layer.m_Bindings.Emplace(input::EKeyboard::Q, strMoveDown);
		layer.m_Bindings.Emplace(input::EKeyboard::E, strMoveRight);
		layer.m_Bindings.Emplace(input::EKeyboard::W, strMoveForward);
		layer.m_Bindings.Emplace(input::EKeyboard::S, strMoveBackward);
		layer.m_Bindings.Emplace(input::EMouse::Right, strRotate);
		layer.m_Bindings.Emplace(input::EKeyboard::Shift_L, strSpeedUpA);
		layer.m_Bindings.Emplace(input::EKeyboard::Shift_R, strSpeedUpA);
		layer.m_Bindings.Emplace(input::EKeyboard::Control_L, strSpeedUpB);
		layer.m_Bindings.Emplace(input::EKeyboard::Control_L, strSpeedUpB);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInputGuid, layer);
	}

	if (count == 0 && world.HasAny<ecs::query::Removed<eng::camera::Move3DComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInputGuid);
	}

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Move3DComponent, const eng::camera::ProjectionComponent>;

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
		{
			const auto& input = world.ReadResource<eng::InputManager>();

			Vector3f direction = Vector3f::Zero;
			if (input.IsPressed(strMoveLeft))
				direction.x -= 1.f;
			if (input.IsPressed(strMoveRight))
				direction.x += 1.f;
			if (input.IsPressed(strMoveDown))
				direction.y -= 1.f;
			if (input.IsPressed(strMoveUp))
				direction.y += 1.f;
			if (input.IsPressed(strMoveForward))
				direction.z += 1.f;
			if (input.IsPressed(strMoveBackward))
				direction.z -= 1.f;

			float speed = cameraSettings.m_TranslateSpeed * gameTime.m_DeltaTime;
			if (input.IsPressed(strSpeedUpA))
				speed *= 3.f;
			if (input.IsPressed(strSpeedUpB))
				speed *= 5.f;

			const Quaternion rotation = Quaternion::FromRotator(readTransform.m_Rotate);
			Vector3f translate = readTransform.m_Translate;
			translate += (direction * speed) * rotation;

			if (!math::IsNearly(readTransform.m_Translate, translate))
			{
				auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				writeTransform.m_Translate = translate;
			}

			if (input.IsPressed(strRotate))
			{
				Rotator rotator = Rotator::Zero;
				rotator.m_Pitch = -input.m_MouseDelta.y * cameraSettings.m_RotateSpeed.m_Pitch;
				rotator.m_Yaw = -input.m_MouseDelta.x * cameraSettings.m_RotateSpeed.m_Yaw;

				// #todo: V-Sync causes the game to rotate faster than when it is disabled
				auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				writeTransform.m_Rotate += rotator;
			}
		}
	}
}