#include "EnginePCH.h"
#include "Engine/CameraPanningSystem.h"

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

void eng::camera::PanningSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Pan3DComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& projection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);

		for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

			if (input.IsKeyHeld(input::EMouse::Left))
			{
				const Vector3f worldPosA = eng::camera::ScreenToWorld(
					Vector2f::Zero,
					projection.m_Projection,
					Matrix4x4::Identity);
				const Vector3f worldPosB = eng::camera::ScreenToWorld(
					input.m_MouseDelta,
					projection.m_Projection,
					Matrix4x4::Identity);

				auto& transform = world.WriteComponent<eng::TransformComponent>(cameraEntity);

				const Quaternion rotation = Quaternion::FromRotator(transform.m_Rotate);
				const Vector3f delta = (worldPosB - worldPosA) * rotation;
				transform.m_Translate += delta;
			}
		}
	}
}