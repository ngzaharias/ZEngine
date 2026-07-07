#include "RenderPCH.h"
#include "Render/RenderDockspaceSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Render/RenderDockspaceComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

void render::DockspaceSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_Offset = Vector2f(0.f, 50.f);
	constexpr ImGuiDockNodeFlags s_DockspaceFlags =
		ImGuiDockNodeFlags_NoDockingOverCentralNode |
		ImGuiDockNodeFlags_PassthruCentralNode;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	const auto& rDockspace = world.ReadComponent<render::DockspaceComponent>();
	if (rDockspace.m_Id == 0)
	{
		auto& wDockspace = world.WriteComponent<render::DockspaceComponent>();
		wDockspace.m_Id = ImGui::GetID("editor::DockSpaceId");
	}
	else
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos + s_Offset);
		ImGui::SetNextWindowSize(viewport->WorkSize - s_Offset);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("##dockspace", nullptr, s_WindowFlags);

		ImGui::DockSpace(rDockspace.m_Id, ImVec2(0, 0), s_DockspaceFlags);

		ImGui::End();
		ImGui::PopStyleVar(3);

		ImGuiDockNode* root = ImGui::DockBuilderGetNode(rDockspace.m_Id);
		ImGuiDockNode* central = root->CentralNode;
		if (rDockspace.m_Pos != Vector2f(central->Pos) || rDockspace.m_Size != Vector2f(central->Size))
		{
			auto& wDockspace = world.WriteComponent<render::DockspaceComponent>();
			wDockspace.m_Pos = central->Pos;
			wDockspace.m_Size = central->Size;
		}
	}
}