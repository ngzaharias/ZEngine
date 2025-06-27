#include "EnginePCH.h"
#include "Engine/ImguiManager.h"

#include "Engine/Window.h"
#include "Engine/GLFW/Window.h"

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imnodes/imnodes.h"
#include "imguizmo/ImGuizmo.h"

namespace
{
	bool AreViewportsEnabled()
	{
		const ImGuiIO& io = ImGui::GetIO();
		return (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;
	}
}

void eng::ImguiManager::Initialise(const eng::Window* window)
{
	PROFILE_FUNCTION();
	m_Window = window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigDockingWithShift = true;
	io.ConfigDockingTransparentPayload = true;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io.ConfigViewportsNoDecoration = true;
	//io.ConfigViewportsNoDefaultParent = true;
	io.ConfigDragClickToInputText = false;
	io.FontGlobalScale = 1.f;

	if (const auto* glfwWindow = dynamic_cast<const glfw::Window*>(window))
	{
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// style
	{
		auto& colours = ImGui::GetStyle().Colors;
		colours[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colours[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colours[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colours[ImGuiCol_HeaderActive] = ImVec4{ 0.44f, 0.f, 0.34f, 1.0f };

		// Buttons
		colours[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colours[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colours[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colours[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colours[ImGuiCol_TabSelected] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colours[ImGuiCol_TabDimmed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TabDimmedSelected] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colours[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colours[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Modal
		colours[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.f, 0.f, 0.f, 0.5f };
	}
}

void eng::ImguiManager::Shutdown()
{
	PROFILE_FUNCTION();

	if (!m_Window)
		return;

	if (const auto* glfwWindow = dynamic_cast<const glfw::Window*>(m_Window))
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	ImNodes::DestroyContext();
	ImGui::DestroyContext();
}

void eng::ImguiManager::PreUpdate()
{
	PROFILE_FUNCTION();

	if (!m_Window)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void eng::ImguiManager::PostUpdate()
{
	PROFILE_FUNCTION();

	if (!m_Window)
		return;

	// render
	{
		PROFILE_CUSTOM("ImGui::Render");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// viewports
	if (AreViewportsEnabled())
	{
		PROFILE_CUSTOM("ImGui::UpdatePlatformWindows");
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		if (const auto* glfwWindow = dynamic_cast<const glfw::Window*>(m_Window))
			glfwMakeContextCurrent(glfwWindow->GetWindow());
	}

	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}