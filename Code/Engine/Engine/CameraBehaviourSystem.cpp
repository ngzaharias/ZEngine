#include "EnginePCH.h"
#include "Engine/CameraBehaviourSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Screen.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Math/Common.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"

void eng::camera::BehaviourSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::BehaviourComponent, eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::BehaviourComponent>(cameraEntity);
		switch (cameraComponent.m_Behaviour)
		{
		case eng::camera::EBehaviour::Static:
			BehaviourStatic(world, gameTime, cameraEntity);
			break;
		case eng::camera::EBehaviour::Free2D:
			BehaviourFree2D(world, gameTime, cameraEntity);
			break;
		case eng::camera::EBehaviour::Free3D:
			BehaviourFree3D(world, gameTime, cameraEntity);
			break;
		}
	}
}

void eng::camera::BehaviourSystem::BehaviourFree2D(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;
	
	const auto& cameraBehaviour = world.ReadComponent<eng::camera::BehaviourComponent>(entity);
	const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(entity);

	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		const auto& inputComponent = world.ReadComponent<eng::InputComponent>(inputEntity);
		const auto& transformComponent = world.ReadComponent<eng::TransformComponent>(entity);
		Vector3f translate = transformComponent.m_Translate;

		// panning wasd
		{
			float speed = cameraSettings.m_TranslateSpeed * gameTime.m_DeltaTime;
			Vector3f direction = Vector3f::Zero;
			if (inputComponent.IsKeyHeld(input::EKeyboard::W))
				direction.y += 1.f;
			if (inputComponent.IsKeyHeld(input::EKeyboard::A))
				direction.x -= 1.f;
			if (inputComponent.IsKeyHeld(input::EKeyboard::S))
				direction.y -= 1.f;
			if (inputComponent.IsKeyHeld(input::EKeyboard::D))
				direction.x += 1.f;

			if (inputComponent.IsKeyHeld(input::EKeyboard::Shift_L))
				speed *= 3.f;
			if (inputComponent.IsKeyHeld(input::EKeyboard::Control_L))
				speed *= 5.f;

			if (direction != Vector3f::Zero)
				direction.Normalize();

			const Vector3f delta = (direction * speed) * Quaternion::FromRotator(transformComponent.m_Rotate);
			translate += delta;
		}

		core::VariantMatch(cameraProjection.m_Projection,
			[&](const eng::camera::Orthographic& data)
			{
				// panning clamp
				{
					const float aspect = Screen::width / Screen::height;
					const Vector2f rangeMin = cameraBehaviour.m_FrustrumEdgeMin.XY();
					const Vector2f rangeMax = cameraBehaviour.m_FrustrumEdgeMax.XY();
					const Vector2f rangeHalf = (rangeMax - rangeMin) * 0.5f;
					const Vector2f frustrumHalf = Vector2f(data.m_Size * aspect, data.m_Size) * 0.5f;

					const Vector2f clamped = math::Max(Vector2f::Zero, rangeHalf - frustrumHalf);
					const Vector2f clampedCen = rangeMin + rangeHalf;
					const Vector2f clampedMin = clampedCen - clamped;
					const Vector2f clampedMax = clampedCen + clamped;

					translate.x = math::Clamp(translate.x, clampedMin.x, clampedMax.x);
					translate.y = math::Clamp(translate.y, clampedMin.y, clampedMax.y);
				}
			},
			[&](const eng::camera::Perspective& data)
			{
			});

		if (!IsNearly(transformComponent.m_Translate, translate))
		{
			auto& transform = world.WriteComponent<eng::TransformComponent>(entity);
			transform.m_Translate = translate;
		}
	}
}

void eng::camera::BehaviourSystem::BehaviourFree3D(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		const auto& inputComponent = world.ReadComponent<eng::InputComponent>(inputEntity);
		auto& transformComponent = world.WriteComponent<eng::TransformComponent>(entity);

		float translateSpeed = cameraSettings.m_TranslateSpeed * gameTime.m_DeltaTime;
		Vector3f translate = Vector3f::Zero;
		Vector3f translateDir = Vector3f::Zero;
		if (inputComponent.IsKeyHeld(input::EKeyboard::A))
			translateDir.x -= 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::D))
			translateDir.x += 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::Q))
			translateDir.y -= 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::E))
			translateDir.y += 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::W))
			translateDir.z += 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::S))
			translateDir.z -= 1.f;

		if (inputComponent.IsKeyHeld(input::EKeyboard::Shift_L))
			translateSpeed *= 3.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::Control_L))
			translateSpeed *= 5.f;

		if (translateDir != Vector3f::Zero)
			translateDir.Normalize();
		translate = (translateDir * translateSpeed) * Quaternion::FromRotator(transformComponent.m_Rotate);
		transformComponent.m_Translate += translate;

		if (inputComponent.IsKeyHeld(input::EMouse::Middle))
		{
			Rotator rotator = Rotator::Zero;
			rotator.m_Pitch = -inputComponent.m_MouseDelta.y * cameraSettings.m_RotateSpeed.m_Pitch;
			rotator.m_Yaw = -inputComponent.m_MouseDelta.x * cameraSettings.m_RotateSpeed.m_Yaw;

			// #todo: V-Sync causes the game to rotate faster than when it is disabled
			transformComponent.m_Rotate += rotator;
		}
	}
}

void eng::camera::BehaviourSystem::BehaviourStatic(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
}