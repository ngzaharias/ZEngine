#include "EnginePCH.h"
#include "Engine/CameraMove2DSystem.h"

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
	const str::Name strMoveDown = str::Name::Create("CameraMove2D_MoveDown");
	const str::Name strMoveLeft = str::Name::Create("CameraMove2D_MoveLeft");
	const str::Name strMoveRight = str::Name::Create("CameraMove2D_MoveRight");
	const str::Name strMoveUp = str::Name::Create("CameraMove2D_MoveUp");
	const str::Name strSpeedUpA = str::Name::Create("CameraMove2D_SpeedUpA");
	const str::Name strSpeedUpB = str::Name::Create("CameraMove2D_SpeedUpB");
}

void eng::camera::Move2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const int32 count = world.Query<ecs::query::Include<eng::camera::Move2DComponent>>().GetCount();
	if (count == 1 && world.HasAny<ecs::query::Added<eng::camera::Move2DComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(input::EKeyboard::W, strMoveUp);
		layer.m_Bindings.Emplace(input::EKeyboard::A, strMoveLeft);
		layer.m_Bindings.Emplace(input::EKeyboard::S, strMoveDown);
		layer.m_Bindings.Emplace(input::EKeyboard::D, strMoveRight);
		layer.m_Bindings.Emplace(input::EKeyboard::Shift_L, strSpeedUpA);
		layer.m_Bindings.Emplace(input::EKeyboard::Shift_R, strSpeedUpA);
		layer.m_Bindings.Emplace(input::EKeyboard::Control_L, strSpeedUpB);
		layer.m_Bindings.Emplace(input::EKeyboard::Control_L, strSpeedUpB);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInputGuid, layer);
	}

	if (count == 0 && world.HasAny<ecs::query::Removed<eng::camera::Move2DComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInputGuid);
	}

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Move2DComponent, const eng::camera::ProjectionComponent>;

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	for (const ecs::Entity& entity : world.Query<CameraQuery>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();

		Vector3f direction = Vector3f::Zero;
		direction.y += input.GetValue(strMoveUp);
		direction.y -= input.GetValue(strMoveDown);
		direction.x -= input.GetValue(strMoveLeft);
		direction.x += input.GetValue(strMoveRight);
		if (direction != Vector3f::Zero)
			direction.Normalize();

		float speed = cameraSettings.m_TranslateSpeed * gameTime.m_DeltaTime;
		if (input.IsHeld(strSpeedUpA))
			speed *= 3.f;
		if (input.IsHeld(strSpeedUpB))
			speed *= 5.f;

		if (!math::IsNearly(direction, Vector3f::Zero))
		{
			auto& transform = world.WriteComponent<eng::TransformComponent>(entity);
			const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
			transform.m_Translate += (direction * speed) * rotation;
		}
	}
}