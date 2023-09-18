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

#include <GameClient/ProjectileComponents.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_user.h>

#include "GameDebug/MenuBarComponents.h"

namespace
{
	str::String ToLabel(const char* label, const int32 windowId)
	{
		return std::format("{}: {}", label, windowId);
	}

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	using World = dbg::TrajectorySystem::World;
	void DrawInspector(World& world, dbg::TrajectoryWindowComponent& component)
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
			ImGui::SameLine();
			if (ImGui::Button("Spawn"))
			{
				constexpr float s_Length = 1000.f;

				path::Points path;

				float distance = 0.f;
				for (int32 i = 0; i < component.m_Positions.GetCount(); ++i)
				{
					const Vector3f positionI = Vector3f(component.m_Positions[i].x, 0.f, component.m_Positions[i].y) * s_Length;
					if (i != 0)
					{
						const Vector3f positionJ = Vector3f(component.m_Positions[i - 1].x, 0.f, component.m_Positions[i-1].y) * s_Length;
						distance += Vector3f::Distance(positionJ, positionI);
					}

					path.m_Positions.Append(positionI);
					path.m_Distances.Append(distance);
				}

				speed::Constant velocity;
				velocity.m_Speed = 1000.f;

				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<projectile::SpawnRequestComponent>(entity);
				requestComponent.m_Trajectory = path::Trajectory(path);
				requestComponent.m_Velocity = velocity;
				requestComponent.m_Scale = Vector3f(0.1f);
				requestComponent.m_Lifetime = 5.f;
			}

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
		windowComponent.m_WindowId = !m_UnusedIds.IsEmpty() ? m_UnusedIds.Pop() : m_NextId++;
		windowComponent.m_Positions.Append(Vector2f(0.f, 0.f));
		windowComponent.m_Positions.Append(Vector2f(0.1f, 0.1f));
		windowComponent.m_Positions.Append(Vector2f(-0.15f, 0.15f));
		windowComponent.m_Positions.Append(Vector2f(0.3f, 0.3f));
		windowComponent.m_Positions.Append(Vector2f(-0.35f, 0.4f));
		windowComponent.m_Positions.Append(Vector2f(0.5f, 0.5f));
		windowComponent.m_Positions.Append(Vector2f(-0.2f, 0.6f));
		windowComponent.m_Positions.Append(Vector2f(0.1f, 0.7f));
		windowComponent.m_Positions.Append(Vector2f(0.f, 1.f));
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<dbg::TrajectoryWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<dbg::TrajectoryWindowComponent>(windowEntity);

		ImGuiDockNodeFlags dockNodeFlags = 0;
		dockNodeFlags |= ImGuiDockNodeFlags_NoCloseButton;
		//dockNodeFlags |= ImGuiDockNodeFlags_NoSplit;
		//dockNodeFlags |= ImGuiDockNodeFlags_NoTabBar;
		dockNodeFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

		const str::String dockspaceLabel = ToLabel("Trajectory Debugger", windowComponent.m_WindowId);
		const str::String inspectorLabel = ToLabel("Inspector", windowComponent.m_WindowId);
		const str::String plotterLabel = ToLabel("Plotter", windowComponent.m_WindowId);
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
			DrawInspector(world, windowComponent);
		ImGui::End();

		if (ImGui::Begin(plotterLabel.c_str()))
			DrawPlotter(windowComponent);
		ImGui::End();

		if (!isOpen)
		{
			m_UnusedIds.Append(windowComponent.m_WindowId);
			world.DestroyEntity(windowEntity);
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::SpawnRequestComponent>>())
		world.DestroyEntity(entity);
}