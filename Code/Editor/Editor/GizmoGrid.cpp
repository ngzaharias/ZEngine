#include "EditorPCH.h"
#include "Editor/GizmoGrid.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/LinesComponent.h"
#include "Math/Algorithms.h"
#include "Math/HexagonHelpers.h"

namespace
{
	constexpr int32 s_Subdivisions = 10;
	constexpr int32 s_MajorCount = 3;
	constexpr int32 s_MinorCount = s_MajorCount * s_Subdivisions;
	constexpr float s_MajorSize = 500.f;
	constexpr float s_MinorSize = s_MajorSize / s_Subdivisions;
	constexpr Vector4f s_MajorColour = Vector4f(0.4f, 0.4f, 0.4f, 1.f);
	constexpr Vector4f s_MinorColour = Vector4f(0.3f, 0.3f, 0.3f, 1.f);
}

void editor::GizmoGrid::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_FloorGrid;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	if (settings.m_ShowMajor)
	{
		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		for (const Vector2i& gridPos : enumerate::Hexagonal(s_MajorCount))
		{
			const Vector2f worldPos = hexagon::ToWorldPos(gridPos, s_MajorSize);
			linesComponent.AddHexagonH(worldPos.X0Y(), s_MajorSize, s_MajorColour);
		}
	}

	if (settings.m_ShowMinor)
	{
		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		for (const Vector2i& gridPos : enumerate::Hexagonal(s_MinorCount))
		{
			const Vector2f worldPos = hexagon::ToWorldPos(gridPos, s_MinorSize);
			linesComponent.AddHexagonH(worldPos.X0Y(), s_MinorSize, s_MinorColour);
		}
	}
}