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
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/SpringMath.h"

void eng::camera::Zoom2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<eng::camera::ProjectionComponent, const eng::camera::Zoom2DComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& cameraSettings = localSettings.m_Camera;

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& readZoom = world.ReadComponent<eng::camera::Zoom2DComponent>(cameraEntity);
		const auto& readProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		if (std::holds_alternative<eng::camera::Orthographic>(readProjection.m_Projection))
		{
			const auto& readOrtho = std::get<eng::camera::Orthographic>(readProjection.m_Projection);
			for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
			{
				const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);
				if (input.m_ScrollDelta.y != 0)
				{
					float size = readOrtho.m_Size;
					size -= input.m_ScrollDelta.y * cameraSettings.m_ZoomAmount;
					size = math::Clamp(size, readZoom.m_Min, readZoom.m_Max);

					auto& writeZoom = world.WriteComponent<eng::camera::Zoom2DComponent>(cameraEntity);
					writeZoom.m_Target = { size, input.m_MousePosition };
				}

				if (readZoom.m_Target)
				{
					auto& writeProj = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
					auto& writeOrtho = std::get<eng::camera::Orthographic>(writeProj.m_Projection);

					const auto& target = *readZoom.m_Target;
					const Vector3f preZoom = camera::ScreenToWorld(
						target.m_Position,
						readProjection.m_Projection,
						readTransform.ToTransform(),
						resolution);

					writeOrtho.m_Size = math::DamperExact(writeOrtho.m_Size, target.m_Size, cameraSettings.m_ZoomSpeed, gameTime.m_DeltaTime);

					const Vector3f postZoom = camera::ScreenToWorld(
						target.m_Position,
						readProjection.m_Projection,
						readTransform.ToTransform(),
						resolution);

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
}