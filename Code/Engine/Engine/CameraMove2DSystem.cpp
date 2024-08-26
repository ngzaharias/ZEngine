#include "EnginePCH.h"
#include "Engine/CameraMove2DSystem.h"

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

void eng::camera::Move2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Move2DComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
		for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

			Vector3f direction = Vector3f::Zero;
			if (input.IsKeyHeld(input::EKeyboard::W))
				direction.y += 1.f;
			if (input.IsKeyHeld(input::EKeyboard::A))
				direction.x -= 1.f;
			if (input.IsKeyHeld(input::EKeyboard::S))
				direction.y -= 1.f;
			if (input.IsKeyHeld(input::EKeyboard::D))
				direction.x += 1.f;

			if (direction != Vector3f::Zero)
				direction.Normalize();

			float speed = cameraSettings.m_TranslateSpeed * gameTime.m_DeltaTime;
			if (input.IsKeyHeld(input::EKeyboard::Shift_L))
				speed *= 3.f;
			if (input.IsKeyHeld(input::EKeyboard::Control_L))
				speed *= 5.f;

			const Quaternion rotation = Quaternion::FromRotator(readTransform.m_Rotate);
			Vector3f translate = readTransform.m_Translate;
			translate += (direction * speed) * rotation;

			if (!IsNearly(readTransform.m_Translate, translate))
			{
				auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				writeTransform.m_Translate = translate;
			}
		}
	}
}