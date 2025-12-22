#include "EnginePCH.h"
#include "Engine/CameraBound2DSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraBound2DComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraProjectionComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/VectorMath.h"

void eng::camera::Bound2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::Bound2DComponent, const eng::camera::ProjectionComponent>;

	const Vector2u& windowSize = window->GetSize();
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& bound2d = view.ReadRequired<eng::camera::Bound2DComponent>();
		const auto& projection = view.ReadRequired<eng::camera::ProjectionComponent>();
		const auto& readTransform = view.ReadRequired<eng::TransformComponent>();

		if (std::holds_alternative<eng::camera::Orthographic>(projection.m_Projection))
		{
			const auto& orthographic = std::get<eng::camera::Orthographic>(projection.m_Projection);

			const float aspect = (float)windowSize.x / (float)windowSize.y;
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

			if (!math::IsNearly(readTransform.m_Translate, translate))
			{
				auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
				writeTransform.m_Translate = translate;
			}
		}
	}
}