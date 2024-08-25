#include "EnginePCH.h"
#include "Engine/CameraBoundsSystem.h"

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

void eng::camera::BoundsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using CameraQuery = ecs::query::Include<eng::TransformComponent, const eng::camera::ProjectionComponent>;
	using InputQuery = ecs::query::Include<const eng::InputComponent>;

	for (const ecs::Entity& cameraEntity : world.Query<CameraQuery>())
	{
		const auto& behaviour = world.ReadComponent<eng::camera::BehaviourComponent>(cameraEntity);
		const auto& projection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& readTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		for (const ecs::Entity& inputEntity : world.Query<InputQuery>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);

			if (std::holds_alternative<eng::camera::Orthographic>(projection.m_Projection))
			{
				const auto& orthographic = std::get<eng::camera::Orthographic>(projection.m_Projection);

				const float aspect = Screen::width / Screen::height;
				const Vector2f rangeMin = behaviour.m_FrustrumEdgeMin.XY();
				const Vector2f rangeMax = behaviour.m_FrustrumEdgeMax.XY();
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