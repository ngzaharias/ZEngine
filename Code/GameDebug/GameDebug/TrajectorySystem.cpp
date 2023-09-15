#include "GameDebugPCH.h"
#include "GameDebug/TrajectorySystem.h"

#include <Core/Algorithms.h>
#include <Core/Math.h>
#include <Core/Trajectory.h>
#include <Core/Vector.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/ColourHelpers.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_user.h>

#include "GameDebug/MenuBarComponents.h"

namespace
{
	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}##{},{}", label, entity.GetIndex(), entity.GetVersion());
	}

	void DrawInspector(dbg::TrajectoryWindowComponent& component)
	{
		Array<Vector2f>& values = component.m_Positions;
		if (ImGui::CollapsingHeader("m_Positions"))
		{
			ImGui::Indent();
			if (ImGui::Button("Add"))
				values.Append(Vector2f::Zero);
			ImGui::SameLine();
			if (ImGui::Button("Pop"))
				values.Pop();
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
				values.RemoveAll();

			for (auto&& [i, position] : enumerate::Forward(values))
			{
				ImGui::PushID(i);
				ImGui::DragFloat2("", &position.x);
				ImGui::PopID();
			}
		}
	}

	void DrawPlotter(dbg::TrajectoryWindowComponent& component)
	{
		Array<Vector2f>& values = component.m_Positions;

		if (ImGui::Button("Normalize"))
		{
			Vector2f range_min = Vector2f(+FLT_MAX);
			Vector2f range_max = Vector2f(-FLT_MAX);
			for (const Vector2f& value : values)
			{
				range_min = Vector2f::Min(range_min, value);
				range_max = Vector2f::Max(range_max, value);
			}

			for (Vector2f& value : values)
			{
				value.x = math::Remap(value.x, range_min.x, range_max.x, 0.f, 1.f);
				value.y = math::Remap(value.y, range_min.y, range_max.y, 0.f, 1.f);
			}
		}

		ImGui::PushItemWidth(-1.f);
		imgui::PlotLines("##plotter", (ImVec2*)values.GetData(), values.GetCount(), ImVec2(0, ImGui::GetContentRegionAvail().y));
	}
}

void dbg::TrajectorySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 350.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::TrajectoryWindowRequestComponent>>())
	{
		auto& windowComponent = world.AddComponent<dbg::TrajectoryWindowComponent>(world.CreateEntity());
		windowComponent.m_Positions.Append(Vector2f::Zero);
		windowComponent.m_Positions.Append(Vector2f::One * 100.f);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<dbg::TrajectoryWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<dbg::TrajectoryWindowComponent>(windowEntity);

		ImGuiDockNodeFlags dockNodeFlags = 0;
		dockNodeFlags |= ImGuiDockNodeFlags_NoCloseButton;
		//dockNodeFlags |= ImGuiDockNodeFlags_NoSplit;
		//dockNodeFlags |= ImGuiDockNodeFlags_NoTabBar;
		dockNodeFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

		const str::String dockspaceLabel = ToLabel("Trajectory Debugger", windowEntity);
		const str::String inspectorLabel = ToLabel("Inspector", windowEntity);
		const str::String plotterLabel = ToLabel("Plotter", windowEntity);
		const ImGuiID dockspaceId = ImGui::GetID(dockspaceLabel.c_str());

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(dockspaceLabel.c_str(), &isOpen))
		{
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID leftId, rightId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.5f, &leftId, &rightId);
				ImGui::DockBuilderDockWindow(inspectorLabel.c_str(), leftId);
				ImGui::DockBuilderDockWindow(plotterLabel.c_str(), rightId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), dockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(inspectorLabel.c_str()))
			DrawInspector(windowComponent);
		ImGui::End();

		if (ImGui::Begin(plotterLabel.c_str()))
			DrawPlotter(windowComponent);
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}