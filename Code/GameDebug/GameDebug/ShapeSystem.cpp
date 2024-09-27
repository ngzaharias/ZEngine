#include "GameDebugPCH.h"
#include "GameDebug/ShapeSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/ShapeComponents.h"
#include "Math/CollisionMath.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_graph.h>
#include <imgui/imgui_user.h>

namespace
{
	constexpr ImVec4 s_Red = ImVec4(1.f, 0.f, 0.f, 1.f);
	constexpr ImVec4 s_White = ImVec4(1.f, 1.f, 1.f, 1.f);

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	const char* ToName(const dbg::Shape& value)
	{
		return core::VariantMatch(value,
			[](const AABB2f&) { return "AABB2f"; },
			[](const Circle2f&) { return "Circle2f"; },
			[](const Line2f&) { return "Line2f"; },
			[](const OBB2f&) { return "OBB2f"; },
			[](const Ray2f&) { return "Ray2f"; },
			[](const Segment2f&) { return "Segment2f"; },
			[](const Triangle2f&) { return "Triangle2f"; });
	}

	bool IsColliding(const dbg::Shape& a, const dbg::Shape& b)
	{
		return false;
		//return std::visit([](const auto& a, const auto& b) -> bool
		//	{
		//		return math::IsOverlapping(a, b);
		//	}, a, b);
	}

	void InspectShape(dbg::Shape& value)
	{
		ImGui::Indent();

		const char* preview = ToName(value);
		if (ImGui::BeginCombo("Shape", preview))
		{
			if (ImGui::Selectable("AABB2f"))
				value = AABB2f(-Vector2f::One, +Vector2f::One);
			if (ImGui::Selectable("Circle2f"))
				value = Circle2f(Vector2f::Zero, 1.f);
			if (ImGui::Selectable("Line2f"))
				value = Line2f(-Vector2f::One, +Vector2f::One);
			if (ImGui::Selectable("OBB2f"))
				value = OBB2f(Vector2f(-1.f, -1.f), Vector2f(-1.f, +1.f), Vector2f(+1.f, +1.f), Vector2f(+1.f, -1.f));
			if (ImGui::Selectable("Ray2f"))
				value = Ray2f(Vector2f::Zero, Vector2f::One);
			if (ImGui::Selectable("Segment2f"))
				value = Segment2f(-Vector2f::One, +Vector2f::One);
			if (ImGui::Selectable("Triangle2f"))
				value = Triangle2f(Vector2f(-1.f, -1.f), Vector2f(0.f, 1.f), Vector2f(+1.f, -1.f));
			ImGui::EndCombo();
		}

		core::VariantMatch(value,
		[](AABB2f& data)
		{
			imgui::DragVector("m_Min", data.m_Min, 0.1f);
			imgui::DragVector("m_Max", data.m_Max, 0.1f);
		},
		[](Circle2f& data)
		{
			imgui::DragVector("m_Position", data.m_Position, 0.1f);
			ImGui::DragFloat("m_Radius", &data.m_Radius, 0.1f);
		},
		[](Line2f& data)
		{
			imgui::DragVector("m_PointA", data.m_PointA, 0.1f);
			imgui::DragVector("m_PointB", data.m_PointB, 0.1f);
		},
		[](OBB2f& data)
		{
			imgui::DragVector("m_PointA", data.m_Points[0], 0.1f);
			imgui::DragVector("m_PointB", data.m_Points[1], 0.1f);
			imgui::DragVector("m_PointC", data.m_Points[2], 0.1f);
			imgui::DragVector("m_PointD", data.m_Points[3], 0.1f);
		},
		[](Ray2f& data)
		{
			imgui::DragVector("m_Direction", data.m_Direction, 0.1f);
			imgui::DragVector("m_Position", data.m_Position, 0.1f);
		},
		[](Segment2f& data)
		{
			imgui::DragVector("m_PointA", data.m_PointA, 0.1f);
			imgui::DragVector("m_PointB", data.m_PointB, 0.1f);
		},
		[](Triangle2f& data)
		{
			imgui::DragVector("m_PointA", data.m_PointA, 0.1f);
			imgui::DragVector("m_PointB", data.m_PointB, 0.1f);
			imgui::DragVector("m_PointC", data.m_PointC, 0.1f);
		});
		ImGui::Unindent();
	}

	void DrawShape(dbg::Shape& value)
	{
		core::VariantMatch(value,
		[&](auto& data)
		{
			imgui::GraphShape(data);
		});
	}

	using World = dbg::ShapeSystem::World;

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
	};

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<dbg::ShapeWindowComponent>(entity);

		if (ImGui::CollapsingHeader("Shape A"))
		{
			ImGui::PushID("ShapeA");
			InspectShape(window.m_ShapeA);
			ImGui::PopID();
		}

		if (ImGui::CollapsingHeader("Shape B"))
		{
			ImGui::PushID("ShapeB");
			InspectShape(window.m_ShapeB);
			ImGui::PopID();
		}
	}

	void DrawPlotter(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<dbg::ShapeWindowComponent>(entity);

		const ImGuiGraphFlags flags = ImGuiGraphFlags_Grid | ImGuiGraphFlags_TextX | ImGuiGraphFlags_TextY;
		const Vector2f size = ImGui::GetContentRegionAvail();
		if (imgui::BeginGraph("##graph", size, flags))
		{
			const bool isColliding = IsColliding(window.m_ShapeA, window.m_ShapeB);
			if (isColliding)
				ImGui::PushStyleColor(ImGuiCol_PlotLines, s_Red);

			DrawShape(window.m_ShapeA);
			DrawShape(window.m_ShapeB);

			if (isColliding)
				ImGui::PopStyleColor();

			imgui::EndGraph();
		}
	}
}

void dbg::ShapeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::ShapeWindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Collision Tester");

		auto& window = world.AddComponent<dbg::ShapeWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Collision Tester", identifier);
		window.m_InspectorLabel = ToLabel("Inspector", identifier);
		window.m_PlottingLabel = ToLabel("Plotter", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const dbg::ShapeWindowComponent>>())
	{
		const auto& window = world.ReadComponent<dbg::ShapeWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<dbg::ShapeWindowComponent>>())
	{
		auto& window = world.WriteComponent<dbg::ShapeWindowComponent>(windowEntity);

		bool isWindowOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_DockspaceLabel.c_str(), &isWindowOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

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
