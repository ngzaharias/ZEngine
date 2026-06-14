#include "GameDebugPCH.h"
#include "GameDebug/ModelGenSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/ModelGenWindowComponent.h"
#include "GameDebug/ModelGenWindowEvent.h"
#include "Math/Segment.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_graph.h"

void debug::ModelGenSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<const debug::ModelGenWindowEvent>())
		world.AddComponent<debug::ModelGenWindowComponent>(world.CreateEntity());

	for (auto&& view : world.Query<ecs::query::Include<const debug::ModelGenWindowComponent>>())
	{
		bool isWindowOpen = true;

		auto& window = view.WriteRequired<debug::ModelGenWindowComponent>();
		if (ImGui::Begin("ModelGen"))
		{
			constexpr float sqrt3 = SQUARE_ROOT_THREE;

			float& radius = window.m_Radius;
			const Vector2f a = Vector2f(-radius * 0.5f, +radius * 0.5f * sqrt3);
			const Vector2f b = Vector2f(+radius * 0.5f, +radius * 0.5f * sqrt3);
			const Vector2f c = Vector2f(+radius, 0.f);
			const Vector2f d = Vector2f(+radius * 0.5f, -radius * 0.5f * sqrt3);
			const Vector2f e = Vector2f(-radius * 0.5f, -radius * 0.5f * sqrt3);
			const Vector2f f = Vector2f(-radius, 0.f);

			ImGui::DragFloat("Radius", &radius);
			ImGui::Text("A: %.3f, %.3f", a.x, a.y);
			ImGui::Text("B: %.3f, %.3f", b.x, b.y);
			ImGui::Text("C: %.3f, %.3f", c.x, c.y);
			ImGui::Text("D: %.3f, %.3f", d.x, d.y);
			ImGui::Text("E: %.3f, %.3f", e.x, e.y);
			ImGui::Text("F: %.3f, %.3f", f.x, f.y);

			ImGui::Text("Width: %.3f", radius * 2.f);
			ImGui::Text("Height: %.3f", radius * sqrt3);

			const ImGuiGraphFlags flags = ImGuiGraphFlags_Grid | ImGuiGraphFlags_TextX | ImGuiGraphFlags_TextY;
			const Vector2f size = ImGui::GetContentRegionAvail();
			if (imgui::BeginGraph("##graph", size, flags))
			{
				Segment2f lineAB = Segment2f(a, b);
				Segment2f lineBC = Segment2f(b, c);
				Segment2f lineCD = Segment2f(c, d);
				Segment2f lineDE = Segment2f(d, e);
				Segment2f lineEF = Segment2f(e, f);
				Segment2f lineFA = Segment2f(f, a);

				imgui::GraphShape(lineAB); imgui::GraphText(a, "A");
				imgui::GraphShape(lineBC); imgui::GraphText(b, "B");
				imgui::GraphShape(lineCD); imgui::GraphText(c, "C");
				imgui::GraphShape(lineDE); imgui::GraphText(d, "D");
				imgui::GraphShape(lineEF); imgui::GraphText(e, "E");
				imgui::GraphShape(lineFA); imgui::GraphText(f, "F");

				imgui::EndGraph();
			}

			ImGui::End();
		}


		if (!isWindowOpen)
			world.DestroyEntity(view);
	}
}