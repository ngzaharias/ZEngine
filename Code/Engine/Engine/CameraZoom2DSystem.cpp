#include "EnginePCH.h"
#include "Engine/CameraZoom2DSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/SettingsComponents.h"
#include "Math/Math.h"

void eng::camera::Zoom2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using CameraQuery = ecs::query::Include<
		eng::camera::ProjectionComponent,
		const eng::camera::BehaviourComponent>;

	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<CameraQuery>>())
	{
		const auto& readBehaviour = world.ReadComponent<eng::camera::BehaviourComponent>(cameraEntity);
		const auto& readProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);

		for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
		{
			const auto& inputComponent = world.ReadComponent<eng::InputComponent>(inputEntity);
			if (std::holds_alternative<eng::camera::Orthographic>(readProjection.m_Projection))
			{
				const auto& readOrthographic = std::get<eng::camera::Orthographic>(readProjection.m_Projection);

				float size = readOrthographic.m_Size;
				size -= inputComponent.m_ScrollDelta.y * cameraSettings.m_ZoomSpeed * gameTime.m_DeltaTime;
				size = math::Clamp(size, readBehaviour.m_ZoomMin, readBehaviour.m_ZoomMax);

				if (size != readOrthographic.m_Size)
				{
					auto& writeProjection = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
					auto& writeOrthographic = std::get<eng::camera::Orthographic>(writeProjection.m_Projection);
					writeOrthographic.m_Size = size;
				}
			}
		}
	}
}