#include "EditorPCH.h"
#include "Editor/GizmoCrosshair.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/CameraComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/Quaternion.h"

void editor::GizmoCrosshair::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_Crosshair;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& transform = world.ReadComponent<eng::TransformComponent>(entity);

		const Quaternion rotate = Quaternion::FromRotator(transform.m_Rotate);
		const Vector3f& translate = transform.m_Translate;
		const Vector3f right = Vector3f::AxisX * rotate;
		const Vector3f upward = Vector3f::AxisY * rotate;
		const Vector3f forward = Vector3f::AxisZ * rotate;

		const Vector3f position = translate + forward * 30.f;

		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		linesComponent.AddLine(
			position - right * 0.1f,
			position + right * 0.1f,
			Vector4f(1.f));
		linesComponent.AddLine(
			position - upward * 0.1f,
			position + upward * 0.1f,
			Vector4f(1.f));
	}
}