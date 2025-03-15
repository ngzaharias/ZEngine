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

namespace
{
	constexpr float s_NaturalLog2 = 0.69314718056f;

	float fast_negexp(float x)
	{
		return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	}

	float damper_exact(float x, float g, float halflife, float dt, float eps = 1e-5f)
	{
		return math::Lerp(x, g, 1.0f - fast_negexp((s_NaturalLog2 * dt) / (halflife + eps)));
	}
}

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
			const auto& readOrthographic = std::get<eng::camera::Orthographic>(readProjection.m_Projection);
			for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
			{
				const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

				float size = readOrthographic.m_Size;
				size -= input.m_ScrollDelta.y * cameraSettings.m_ZoomAmount;
				size = math::Clamp(size, readZoom.m_Min, readZoom.m_Max);

				if (size != readOrthographic.m_Size)
				{
					auto& writeProjection = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
					auto& writeOrthographic = std::get<eng::camera::Orthographic>(writeProjection.m_Projection);
					auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);

					const Vector3f preZoom = camera::ScreenToWorld(
						input.m_MousePosition,
						readProjection.m_Projection,
						readTransform.ToTransform(),
						resolution);

					writeOrthographic.m_Size = size;

					const Vector3f postZoom = camera::ScreenToWorld(
						input.m_MousePosition,
						readProjection.m_Projection,
						readTransform.ToTransform(),
						resolution);

					// we calculate the delta on the mouse pos and add it back onto the translate
					writeTransform.m_Translate += preZoom - postZoom;
				}
			}

		}
	}
}