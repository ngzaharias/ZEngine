#include "CameraPCH.h"
#include "Camera/CameraMove3DSystem.h"

#include "Camera/CameraMove3DComponent.h"
#include "Camera/CameraSettingsSingleton.h"
#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/TransformComponent.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/VectorMath.h"

namespace
{
	const str::Name strInput = str::Name::Create("CameraMove3D");
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

void camera::Move3DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<camera::Move3DComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strMoveLeft,     input::EKey::A);
		layer.m_Bindings.Emplace(strMoveRight,    input::EKey::D);
		layer.m_Bindings.Emplace(strMoveDown,     input::EKey::Q);
		layer.m_Bindings.Emplace(strMoveUp,       input::EKey::E);
		layer.m_Bindings.Emplace(strMoveForward,  input::EKey::W);
		layer.m_Bindings.Emplace(strMoveBackward, input::EKey::S);
		layer.m_Bindings.Emplace(strRotate,       input::EKey::Mouse_Right);
		layer.m_Bindings.Emplace(strSpeedUpA,     input::EKey::Shift_L);
		layer.m_Bindings.Emplace(strSpeedUpA,     input::EKey::Shift_R);
		layer.m_Bindings.Emplace(strSpeedUpB,     input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSpeedUpB,     input::EKey::Control_L);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<camera::Move3DComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	using CameraQuery = ecs::query
		::Include<
		eng::TransformComponent,
		const camera::Move3DComponent,
		const eng::ActiveComponent,
		const eng::CameraComponent>;

	const auto& settings = world.ReadSingleton<camera::SettingsSingleton>();
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		const auto& transform = view.ReadRequired<eng::TransformComponent>();

		Vector3f direction = Vector3f::Zero;
		direction.x += input.GetValue(strMoveRight);
		direction.x -= input.GetValue(strMoveLeft);
		direction.y += input.GetValue(strMoveUp);
		direction.y -= input.GetValue(strMoveDown);
		direction.z += input.GetValue(strMoveForward);
		direction.z -= input.GetValue(strMoveBackward);

		float speed = settings.m_TranslateSpeed * gameTime.m_DeltaTime;
		if (input.IsHeld(strSpeedUpA))
			speed *= 3.f;
		if (input.IsHeld(strSpeedUpB))
			speed *= 5.f;

		const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
		Vector3f translate = transform.m_Translate;
		translate += (direction * speed) * rotation;

		if (!math::IsNearly(transform.m_Translate, translate))
		{
			auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
			writeTransform.m_Translate = translate;
		}

		if (input.IsHeld(strRotate))
		{
			Rotator rotator = Rotator::Zero;
			rotator.m_Pitch = -input.m_MouseDelta.y * settings.m_RotateSpeed.m_Pitch;
			rotator.m_Yaw = -input.m_MouseDelta.x * settings.m_RotateSpeed.m_Yaw;

			// #todo: V-Sync causes the game to rotate faster than when it is disabled
			auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
			writeTransform.m_Rotate += rotator;
		}
	}
}