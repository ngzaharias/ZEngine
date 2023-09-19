#include "EnginePCH.h"
#include "Engine/CameraSystem.h"

#include <Core/GameTime.h>
#include <Core/Quaternion.h>
#include <Core/Rotator.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/CameraComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/TransformComponent.h"

namespace
{
	constexpr float s_TranslateSpeed = 1000.f;
	constexpr Rotator s_RotateSpeed = Rotator(0.2f);
}

void eng::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, eng::TransformComponent>>())
	{
		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
			auto& transformComponent = world.GetComponent<eng::TransformComponent>(cameraEntity);

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
}