#include "EnginePCH.h"
#include "Engine/CameraZoom2DSystem.h"

#include "Core/GameTime.h"
#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/CameraZoom2DComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/SettingsCameraSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/SpringMath.h"

void eng::camera::Zoom2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<eng::camera::ProjectionComponent, const eng::camera::Zoom2DComponent>;

	const auto& cameraSettings = world.ReadSingleton<eng::settings::CameraSingleton>();

	const Vector2u& windowSize = window->GetSize();
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& readZoom = world.ReadComponent<eng::camera::Zoom2DComponent>(cameraEntity);
		const auto& readProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		if (std::holds_alternative<eng::camera::Orthographic>(readProjection.m_Projection))
		{
			const auto& readOrtho = std::get<eng::camera::Orthographic>(readProjection.m_Projection);
			const auto& input = world.ReadResource<eng::InputManager>();
			if (input.m_ScrollDelta.y != 0)
			{
				float size = readOrtho.m_Size;
				size -= input.m_ScrollDelta.y * cameraSettings.m_ZoomAmount;
				size = math::Clamp(size, readZoom.m_Min, readZoom.m_Max);

				auto& writeZoom = world.WriteComponent<eng::camera::Zoom2DComponent>(cameraEntity);
				writeZoom.m_Target = { input.m_MousePosition, size };
			}

			if (readZoom.m_Target)
			{
				auto& writeProj = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
				auto& writeOrtho = std::get<eng::camera::Orthographic>(writeProj.m_Projection);

				const auto& target = *readZoom.m_Target;
				const Vector3f preZoom = camera::ScreenToWorld(
					readProjection.m_Projection,
					readTransform.ToTransform(),
					windowSize,
					target.m_Position);

				writeOrtho.m_Size = math::DamperExact(writeOrtho.m_Size, target.m_Size, cameraSettings.m_ZoomSpeed, gameTime.m_DeltaTime);

				const Vector3f postZoom = camera::ScreenToWorld(
					readProjection.m_Projection,
					readTransform.ToTransform(),
					windowSize,
					target.m_Position);

				// we calculate the delta on the mouse pos and add it back onto the translate
				auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				writeTransform.m_Translate += preZoom - postZoom;
			}

			if (readZoom.m_Target && math::IsNearly(readOrtho.m_Size, readZoom.m_Target->m_Size))
			{
				auto& writeZoom = world.WriteComponent<eng::camera::Zoom2DComponent>(cameraEntity);
				writeZoom.m_Target = {};
			}
		}
	}
}