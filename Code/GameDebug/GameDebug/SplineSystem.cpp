#include "GameDebugPCH.h"
#include "GameDebug/SplineSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/SplineComponents.h"
#include "Math/Algorithms.h"
#include "Math/SplineMath.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_graph.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	using World = dbg::SplineSystem::World;
	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<dbg::SplineWindowComponent>(entity);

		imgui::Inspector inspector;
		if (inspector.Begin("inspector"))
		{
			inspector.Write("Type", window.m_Spline);
			inspector.End();
		}
	}

	void DrawPlotter(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<dbg::SplineWindowComponent>(entity);

		const ImGuiGraphFlags flags = ImGuiGraphFlags_Grid | ImGuiGraphFlags_TextX | ImGuiGraphFlags_TextY;
		const Vector2f size = ImGui::GetContentRegionAvail();
		if (imgui::BeginGraph("##graph", size, flags))
		{
			core::VariantMatch(window.m_Spline,
				[&](dbg::BezierQuadratic& data)
				{
					imgui::GraphPoint(data.c);
					imgui::GraphPoint(data.p1);
					imgui::GraphPoint(data.p2);

					Vector2f p1, p2 = data.p1;
					for (const float t : enumerate::Length(0.1f, 1.f))
					{
						p1 = p2;
						p2 = math::BezierQuadratic(data.p1, data.c, data.p2, t);
						imgui::GraphLine(p1, p2);
					}
				},
				[&](dbg::BezierCubic& data)
				{
					imgui::GraphPoint(data.c1);
					imgui::GraphPoint(data.c2);
					imgui::GraphPoint(data.p1);
					imgui::GraphPoint(data.p2);

					Vector2f p1, p2 = data.p1;
					for (const float t : enumerate::Length(0.1f, 1.f))
					{
						p1 = p2;
						p2 = math::BezierCubic(data.p1, data.c1, data.c2, data.p2, t);
						imgui::GraphLine(p1, p2);
					}
				},
				[&](dbg::CatmullRom& data)
				{
					imgui::GraphPoint(data.c1);
					imgui::GraphPoint(data.c2);
					imgui::GraphPoint(data.p1);
					imgui::GraphPoint(data.p2);

					Vector2f p1, p2 = data.p1;
					for (const float t : enumerate::Length(0.1f, 1.f))
					{
						p1 = p2;
						p2 = math::CatmullRom(data.c1, data.p1, data.p2, data.c2, t);
						imgui::GraphLine(p1, p2);
					}
				},
				[&](dbg::Hermite& data)
				{
					imgui::GraphPoint(data.p1);
					imgui::GraphPoint(data.p2);
					imgui::GraphPoint(data.v1);
					imgui::GraphPoint(data.v2);

					Vector2f p1, p2 = data.p1;
					for (const float t : enumerate::Length(0.1f, 1.f))
					{
						p1 = p2;
						p2 = math::Hermite(data.p1, data.v1, data.p2, data.v2, t);
						imgui::GraphLine(p1, p2);
					}
				});

			imgui::EndGraph();
		}
	}
}

void dbg::SplineSystem::Update(World& world, const GameTime& gameTime)
{
	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::SplineWindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Spline Tester");

		auto& window = world.AddComponent<dbg::SplineWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Spline Tester", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##collision", identifier);
		window.m_PlottingLabel = ToLabel("Plotter##collision", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const dbg::SplineWindowComponent>>())
	{
		const auto& window = world.ReadComponent<dbg::SplineWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<dbg::SplineWindowComponent>>())
	{
		auto& window = world.WriteComponent<dbg::SplineWindowComponent>(windowEntity);

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_DockspaceLabel.c_str(), &isWindowOpen, s_WindowFlags))
		{
			const ImGuiID dockspaceId = ImGui::GetID(window.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID inspectorId, plottingId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.7f, &plottingId, &inspectorId);
				ImGui::DockBuilderDockWindow(window.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(window.m_PlottingLabel.c_str(), plottingId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(window.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(window.m_PlottingLabel.c_str()))
			DrawPlotter(world, windowEntity);
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(windowEntity);
	}
}
