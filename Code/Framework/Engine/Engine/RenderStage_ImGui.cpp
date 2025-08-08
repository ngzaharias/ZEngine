#include "EnginePCH.h"
#include "Engine/RenderStage_ImGui.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

void eng::RenderStage_ImGui::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
