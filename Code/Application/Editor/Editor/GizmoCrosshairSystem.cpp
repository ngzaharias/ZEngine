#include "EditorPCH.h"
#include "Editor/GizmoCrosshairSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Engine/CameraComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/Quaternion.h"

void editor::gizmo::CrosshairSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_Crosshair;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	using Query = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;

	for (auto&& view : world.Query<Query>())
	{
		const auto& transform = view.ReadRequired<eng::TransformComponent>();

		const Quaternion rotate = Quaternion::FromRotator(transform.m_Rotate);
		const Vector3f& translate = transform.m_Translate;
		const Vector3f right = Vector3f::AxisX * rotate;
		const Vector3f upward = Vector3f::AxisY * rotate;
		const Vector3f forward = Vector3f::AxisZ * rotate;

		const Vector3f position = translate + forward * 30.f;

		auto& linesSingleton = world.WriteSingleton<eng::LinesSingleton>();
		linesSingleton.AddLine(
			position - right * 0.1f,
			position + right * 0.1f,
			Colour::White);
		linesSingleton.AddLine(
			position - upward * 0.1f,
			position + upward * 0.1f,
			Colour::White);
	}
}