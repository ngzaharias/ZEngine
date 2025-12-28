#include "CameraPCH.h"
#include "Camera/CameraZoom2DSystem.h"

#include "Camera/CameraZoom2DComponent.h"
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
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/SpringMath.h"

void camera::Zoom2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();
	const auto& cameraSettings = world.ReadSingleton<camera::SettingsSingleton>();

	using CameraQuery = ecs::query
		::Include<
		eng::CameraComponent, 
		const camera::Zoom2DComponent,
		const eng::TransformComponent>;
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& readZoom = view.ReadRequired<camera::Zoom2DComponent>();
		const auto& readCamera = view.ReadRequired<eng::CameraComponent>();
		const auto& readTransform = view.ReadRequired<eng::TransformComponent>();

		if (std::holds_alternative<eng::Orthographic>(readCamera.m_Projection))
		{
			const auto& readOrtho = std::get<eng::Orthographic>(readCamera.m_Projection);
			const auto& input = world.ReadResource<eng::InputManager>();
			if (input.m_ScrollDelta.y != 0)
			{
				float size = readOrtho.m_Size;
				size -= input.m_ScrollDelta.y * cameraSettings.m_ZoomAmount;
				size = math::Clamp(size, readZoom.m_Min, readZoom.m_Max);

				auto& writeZoom = view.WriteRequired<camera::Zoom2DComponent>();
				writeZoom.m_Target = { input.m_MousePosition, size };
			}

			if (readZoom.m_Target)
			{
				auto& writeProj = view.WriteRequired<eng::CameraComponent>();
				auto& writeOrtho = std::get<eng::Orthographic>(writeProj.m_Projection);

				const auto& target = *readZoom.m_Target;
				const Vector3f preZoom = eng::ScreenToWorld(
					readCamera.m_Projection,
					readTransform.ToTransform(),
					windowSize,
					target.m_Position);

				writeOrtho.m_Size = math::DamperExact(writeOrtho.m_Size, target.m_Size, cameraSettings.m_ZoomSpeed, gameTime.m_DeltaTime);

				const Vector3f postZoom = eng::ScreenToWorld(
					readCamera.m_Projection,
					readTransform.ToTransform(),
					windowSize,
					target.m_Position);

				// we calculate the delta on the mouse pos and add it back onto the translate
				auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
				writeTransform.m_Translate += preZoom - postZoom;
			}

			if (readZoom.m_Target && math::IsNearly(readOrtho.m_Size, readZoom.m_Target->m_Size))
			{
				auto& writeZoom = view.WriteRequired<camera::Zoom2DComponent>();
				writeZoom.m_Target = {};
			}
		}
	}
}