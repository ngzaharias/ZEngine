#include "CameraPCH.h"
#include "SharedCamera/CameraBound2DSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Math.h"
#include "Math/VectorMath.h"
#include "SharedCamera/CameraBound2DComponent.h"
#include "SharedCamera/CameraBound2DTemplate.h"

void shared::camera::Bound2DSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using CameraQuery = ecs::query
		::Include<
		eng::TransformComponent,
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const shared::camera::Bound2DComponent>;

	const Vector2u& windowSize = window->GetSize();
	for (auto&& view : world.Query<CameraQuery>())
	{
		const auto& bound2d = view.ReadRequired<shared::camera::Bound2DComponent>();
		const auto& camera = view.ReadRequired<eng::CameraComponent>();
		const auto& transform = view.ReadRequired<eng::TransformComponent>();

		if (std::holds_alternative<eng::Orthographic>(camera.m_Projection))
		{
			const auto& orthographic = std::get<eng::Orthographic>(camera.m_Projection);

			const float aspect = (float)windowSize.x / (float)windowSize.y;
			const Vector2f rangeMin = bound2d.m_Min;
			const Vector2f rangeMax = bound2d.m_Max;
			const Vector2f rangeHalf = (rangeMax - rangeMin) * 0.5f;
			const Vector2f frustrumHalf = Vector2f(orthographic.m_Size * aspect, orthographic.m_Size) * 0.5f;

			const Vector2f clamped = math::Max(Vector2f::Zero, rangeHalf - frustrumHalf);
			const Vector2f clampedCen = rangeMin + rangeHalf;
			const Vector2f clampedMin = clampedCen - clamped;
			const Vector2f clampedMax = clampedCen + clamped;

			Vector3f translate = transform.m_Translate;
			translate.x = math::Clamp(translate.x, clampedMin.x, clampedMax.x);
			translate.y = math::Clamp(translate.y, clampedMin.y, clampedMax.y);

			if (!math::IsNearly(transform.m_Translate, translate))
			{
				auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
				writeTransform.m_Translate = translate;
			}
		}
	}

	using AddedQuery = ecs::query
		::Added<const shared::camera::Bound2DTemplate>
		::Include<const shared::camera::Bound2DTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& cameraTemplate = view.ReadRequired<shared::camera::Bound2DTemplate>();
		auto& cameraComponent = world.AddComponent<shared::camera::Bound2DComponent>(view);
		cameraComponent.m_Min = cameraTemplate.m_Min;
		cameraComponent.m_Max = cameraTemplate.m_Max;
	}

	using UpdatedQuery = ecs::query
		::Updated<
		const shared::camera::Bound2DTemplate>
		::Include<
		shared::camera::Bound2DComponent, 
		const shared::camera::Bound2DTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& cameraTemplate = view.ReadRequired<shared::camera::Bound2DTemplate>();
		auto& cameraComponent = view.WriteRequired<shared::camera::Bound2DComponent>();
		cameraComponent.m_Min = cameraTemplate.m_Min;
		cameraComponent.m_Max = cameraTemplate.m_Max;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const shared::camera::Bound2DTemplate>>())
	{
		world.RemoveComponent<shared::camera::Bound2DComponent>(view);
	}
}