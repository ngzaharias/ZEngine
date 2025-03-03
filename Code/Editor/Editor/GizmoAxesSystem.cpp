#include "EditorPCH.h"
#include "Editor/GizmoAxesSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/LinesComponent.h"

namespace
{
	constexpr float s_Distance = 100000.f;
	constexpr Colour s_ColourX = Colour(0.58f, 0.23f, 0.29f);
	constexpr Colour s_ColourY = Colour(0.38f, 0.52f, 0.16f);
	constexpr Colour s_ColourZ = Colour(0.21f, 0.4f, 0.61f);
}

void editor::gizmo::AxesSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_CoordAxes;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
	if (settings.m_ShowX)
		linesComponent.AddLine(Vector3f::AxisX * -s_Distance, Vector3f::AxisX * s_Distance, s_ColourX);
	if (settings.m_ShowY)
		linesComponent.AddLine(Vector3f::AxisY * -s_Distance, Vector3f::AxisY * s_Distance, s_ColourY);
	if (settings.m_ShowZ)
		linesComponent.AddLine(Vector3f::AxisZ * -s_Distance, Vector3f::AxisZ * s_Distance, s_ColourZ);
}