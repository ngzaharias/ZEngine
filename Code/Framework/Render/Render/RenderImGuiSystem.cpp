#include "RenderPCH.h"
#include "Render/RenderImGuiSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/GLFW/Window.h"
#include "Engine/WindowManager.h"
#include "Engine/Window.h"

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imguizmo/ImGuizmo.h"

namespace
{
	bool AreViewportsEnabled()
	{
		const ImGuiIO& io = ImGui::GetIO();
		return (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;
	}
}

void render::ImGuiSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (AreViewportsEnabled())
	{
		PROFILE_CUSTOM("ImGui::UpdatePlatformWindows");
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		const auto& windowManager = world.ReadResource<eng::WindowManager>();
		const auto* window = windowManager.GetWindow(0);
		if (const auto* glfwWindow = dynamic_cast<const glfw::Window*>(window))
			glfwMakeContextCurrent(glfwWindow->GetWindow());
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}
