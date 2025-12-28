#include "EnginePCH.h"
#include "Engine/ImguiManager.h"

#include "Engine/Window.h"
#include "Engine/GLFW/Window.h"
#include "Input/Key.h"

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imnodes/imnodes.h"
#include "imguizmo/ImGuizmo.h"

namespace
{
	bool IsGamepad(const input::EKey value)
	{
		return value >= input::EKey::Gamepad_A && value <= input::EKey::Gamepad_DPad_L;
	}

	bool IsKeyboard(const input::EKey value)
	{
		return value > input::EKey::None && value < input::EKey::Mouse_1;
	}

	bool IsMouse(const input::EKey value)
	{
		return value >= input::EKey::Mouse_1 && value <= input::EKey::Mouse_8;
	}
}

void eng::ImguiManager::Initialise(const eng::Window& window)
{
	PROFILE_FUNCTION();
	m_Window = &window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigDockingWithShift = true;
	io.ConfigDockingTransparentPayload = true;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io.ConfigViewportsNoDecoration = true;
	//io.ConfigViewportsNoDefaultParent = true;
	io.ConfigDragClickToInputText = false;
	io.FontGlobalScale = 1.f;

	if (const auto* glfwWindow = dynamic_cast<const glfw::Window*>(&window))
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

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
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

void eng::ImguiManager::ProcessInput(
	const Vector2f& mousePos,
	const Vector2f& mouseDelta,
	const Vector2f& scrollDelta,
	Set<input::EKey>& inout_Held,
	Set<input::EKey>& inout_Pressed,
	Set<input::EKey>& inout_Released)
{
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		Set<input::EKey> held = inout_Held;
		for (const input::EKey value : inout_Held)
		{
			if (IsKeyboard(value))
				held.Remove(value);
		}

		Set<input::EKey> pressed = inout_Pressed;
		for (const input::EKey value : inout_Pressed)
		{
			if (IsKeyboard(value))
				pressed.Remove(value);
		}

		Set<input::EKey> released = inout_Released;
		for (const input::EKey value : inout_Released)
		{
			if (IsKeyboard(value))
				released.Remove(value);
		}

		std::swap(held, inout_Held);
		std::swap(pressed, inout_Pressed);
		std::swap(released, inout_Released);
	}

	if (ImGui::GetIO().WantCaptureMouse)
	{
		Set<input::EKey> held = inout_Held;
		for (const input::EKey value : inout_Held)
		{
			if (IsMouse(value))
				held.Remove(value);
		}

		Set<input::EKey> pressed = inout_Pressed;
		for (const input::EKey value : inout_Pressed)
		{
			if (IsMouse(value))
				pressed.Remove(value);
		}

		Set<input::EKey> released = inout_Released;
		for (const input::EKey value : inout_Released)
		{
			if (IsMouse(value))
				released.Remove(value);
		}

		std::swap(held, inout_Held);
		std::swap(pressed, inout_Pressed);
		std::swap(released, inout_Released);
	}
}