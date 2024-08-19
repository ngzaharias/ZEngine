#include "EnginePCH.h"
#include "Engine/CameraBehaviourSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"

namespace
{
	constexpr Rotator s_RotateSpeed = Rotator(0.2f);
	constexpr float s_TranslateSpeed = 1000.f;
	constexpr float s_ZoomSpeed = 3000.f;
}

void eng::camera::BehaviourSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(cameraEntity);
		switch (cameraComponent.m_Behaviour)
		{
		case ::camera::EBehaviour::Static:
			BehaviourStatic(world, gameTime, cameraEntity);
			break;
		case ::camera::EBehaviour::Free2D:
			BehaviourFree2D(world, gameTime, cameraEntity);
			break;
		case ::camera::EBehaviour::Free3D:
			BehaviourFree3D(world, gameTime, cameraEntity);
			break;
		}

	}
}

void eng::camera::BehaviourSystem::BehaviourFree2D(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
		auto& cameraComponent = world.GetComponent<eng::CameraComponent>(entity);
		auto& transformComponent = world.GetComponent<eng::TransformComponent>(entity);

		float translateSpeed = s_TranslateSpeed * gameTime.m_DeltaTime;
		float zoomSpeed = s_ZoomSpeed * gameTime.m_DeltaTime;
		Vector3f translate = Vector3f::Zero;
		Vector3f translateDir = Vector3f::Zero;
		if (inputComponent.IsKeyHeld(input::EKeyboard::W))
			translateDir.y += 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::A))
			translateDir.x -= 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::S))
			translateDir.y -= 1.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::D))
			translateDir.x += 1.f;
		if (inputComponent.IsKeyHeld(input::EMouse::Middle))
		{
			translateDir.x += inputComponent.m_MouseDelta.x;
			translateDir.y -= inputComponent.m_MouseDelta.y;
		}

		if (inputComponent.IsKeyHeld(input::EKeyboard::Shift_L))
			translateSpeed *= 3.f;
		if (inputComponent.IsKeyHeld(input::EKeyboard::Control_L))
			translateSpeed *= 5.f;

		if (translateDir != Vector3f::Zero)
			translateDir.Normalize();
		translate = (translateDir * translateSpeed) * Quaternion::FromRotator(transformComponent.m_Rotate);
		transformComponent.m_Translate += translate;

		core::VariantMatch(cameraComponent.m_Projection,
			[&](::camera::Orthographic& data)
			{
				data.m_Size -= inputComponent.m_ScrollDelta.y * zoomSpeed;
			},
			[&](::camera::Perspective& data)
			{
			});
	}
}

void eng::camera::BehaviourSystem::BehaviourFree3D(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
	{
		const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
		auto& transformComponent = world.GetComponent<eng::TransformComponent>(entity);

		float translateSpeed = s_TranslateSpeed * gameTime.m_DeltaTime;
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

		if (inputComponent.IsKeyHeld(input::EMouse::Right))
		{
			Rotator rotator = Rotator::Zero;
			rotator.m_Pitch = -inputComponent.m_MouseDelta.y * s_RotateSpeed.m_Pitch;
			rotator.m_Yaw = -inputComponent.m_MouseDelta.x * s_RotateSpeed.m_Yaw;

			// #todo: V-Sync enabled rotates faster than when it is disabled
			transformComponent.m_Rotate += rotator;
		}
	}
}

void eng::camera::BehaviourSystem::BehaviourStatic(World& world, const GameTime& gameTime, const ecs::Entity& entity)
{
}