#include "CameraPCH.h"
#include "Camera/CameraMove2DSystem.h"

#include "Camera/CameraMove2DComponent.h"
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
	const str::Name strInput = str::Name::Create("CameraMove2D");
	const str::Name strMoveDown = str::Name::Create("CameraMove2D_MoveDown");
	const str::Name strMoveLeft = str::Name::Create("CameraMove2D_MoveLeft");
	const str::Name strMoveRight = str::Name::Create("CameraMove2D_MoveRight");
	const str::Name strMoveUp = str::Name::Create("CameraMove2D_MoveUp");
	const str::Name strSpeedUpA = str::Name::Create("CameraMove2D_SpeedUpA");
	const str::Name strSpeedUpB = str::Name::Create("CameraMove2D_SpeedUpB");
}

void camera::Move2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<camera::Move2DComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strMoveUp,    input::EKey::W);
		layer.m_Bindings.Emplace(strMoveLeft,  input::EKey::A);
		layer.m_Bindings.Emplace(strMoveDown,  input::EKey::S);
		layer.m_Bindings.Emplace(strMoveRight, input::EKey::D);
		layer.m_Bindings.Emplace(strSpeedUpA,  input::EKey::Shift_L);
		layer.m_Bindings.Emplace(strSpeedUpA,  input::EKey::Shift_R);
		layer.m_Bindings.Emplace(strSpeedUpB,  input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSpeedUpB,  input::EKey::Control_L);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<camera::Move2DComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	using CameraQuery = ecs::query
		::Include<
		eng::TransformComponent, 
		const camera::Move2DComponent, 
		const eng::CameraComponent>;

	const auto& settings = world.ReadSingleton<camera::SettingsSingleton>();
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();

		Vector3f direction = Vector3f::Zero;
		direction.y += input.GetValue(strMoveUp);
		direction.y -= input.GetValue(strMoveDown);
		direction.x -= input.GetValue(strMoveLeft);
		direction.x += input.GetValue(strMoveRight);
		if (direction != Vector3f::Zero)
			direction.Normalize();

		float speed = settings.m_TranslateSpeed * gameTime.m_DeltaTime;
		if (input.IsHeld(strSpeedUpA))
			speed *= 3.f;
		if (input.IsHeld(strSpeedUpB))
			speed *= 5.f;

		if (!math::IsNearly(direction, Vector3f::Zero))
		{
			auto& transform = view.WriteRequired<eng::TransformComponent>();
			const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
			transform.m_Translate += (direction * speed) * rotation;
		}
	}
}