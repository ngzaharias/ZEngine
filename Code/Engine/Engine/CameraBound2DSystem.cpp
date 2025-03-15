#include "EnginePCH.h"
#include "Engine/CameraBound2DSystem.h"

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
#include "Math/Common.h"
#include "Math/Math.h"

void eng::camera::Bound2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Bound2DComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& bound2d = world.ReadComponent<eng::camera::Bound2DComponent>(cameraEntity);
		const auto& projection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

			if (std::holds_alternative<eng::camera::Orthographic>(projection.m_Projection))
			{
				const auto& orthographic = std::get<eng::camera::Orthographic>(projection.m_Projection);

				const float aspect = (float)resolution.x / (float)resolution.y;
				const Vector2f rangeMin = bound2d.m_Min;
				const Vector2f rangeMax = bound2d.m_Max;
				const Vector2f rangeHalf = (rangeMax - rangeMin) * 0.5f;
				const Vector2f frustrumHalf = Vector2f(orthographic.m_Size * aspect, orthographic.m_Size) * 0.5f;

				const Vector2f clamped = math::Max(Vector2f::Zero, rangeHalf - frustrumHalf);
				const Vector2f clampedCen = rangeMin + rangeHalf;
				const Vector2f clampedMin = clampedCen - clamped;
				const Vector2f clampedMax = clampedCen + clamped;

				Vector3f translate = readTransform.m_Translate;
				translate.x = math::Clamp(translate.x, clampedMin.x, clampedMax.x);
				translate.y = math::Clamp(translate.y, clampedMin.y, clampedMax.y);

				if (!IsNearly(readTransform.m_Translate, translate))
				{
					auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
					writeTransform.m_Translate = translate;
				}
			}
		}
	}
}