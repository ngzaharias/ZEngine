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
		ImGuiStyle * style = &ImGui::GetStyle();

		style->WindowPadding            = ImVec2(15, 15);
		style->WindowRounding           = 5.0f;
		style->FramePadding             = ImVec2(5, 5);
		style->FrameRounding            = 4.0f;
		style->ItemSpacing              = ImVec2(12, 8);
		style->ItemInnerSpacing         = ImVec2(8, 6);
		style->IndentSpacing            = 25.0f;
		style->ScrollbarSize            = 12.0f;
		style->ScrollbarRounding        = 9.0f;
		style->GrabMinSize              = 5.0f;
		style->GrabRounding             = 3.0f;

		style->TabRounding              = 4.0f;
		style->TabBarBorderSize         = 1.0f;
		style->TabBorderSize            = 0.0f;
		style->WindowBorderSize         = 0.5f;
		style->ChildBorderSize          = 0.5f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.55f, 0.60f, 1.00f);

		colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.14f, 0.15f, 0.90f);
		colors[ImGuiCol_ChildBg]                = ImVec4(0.13f, 0.14f, 0.15f, 0.90f);

		colors[ImGuiCol_PopupBg]                = ImVec4(0.11f, 0.11f, 0.14f, 0.94f);

		colors[ImGuiCol_Border]                 = ImVec4(0.25f, 0.55f, 0.35f, 0.50f);
		colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.22f, 0.24f, 0.54f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.35f, 0.65f, 0.45f, 0.40f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.52f, 0.80f, 0.09f, 0.67f);

		colors[ImGuiCol_TitleBg]                = ImVec4(0.15f, 0.20f, 0.18f, 1.00f);
		colors[ImGuiCol_TitleBgActive]          = ImVec4(0.20f, 0.45f, 0.35f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.12f, 0.16f, 0.14f, 1.00f);

		colors[ImGuiCol_Tab]                    = ImVec4(0.13f, 0.18f, 0.15f, 1.00f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.20f, 0.40f, 0.30f, 1.00f);
		//colors[ImGuiCol_TabActive]              = ImVec4(0.30f, 0.70f, 0.52f, 1.00f);
		//colors[ImGuiCol_TabUnfocused]           = ImVec4(0.10f, 0.14f, 0.12f, 0.97f);
		//colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.18f, 0.35f, 0.26f, 1.00f);

		colors[ImGuiCol_MenuBarBg]              = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.11f, 0.11f, 0.11f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.33f, 0.35f, 0.37f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.70f, 0.50f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);

		colors[ImGuiCol_CheckMark]              = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);
		colors[ImGuiCol_SliderGrab]             = ImVec4(0.35f, 0.65f, 0.45f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);

		colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.25f, 0.30f, 1.00f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(0.35f, 0.65f, 0.45f, 0.80f);
		colors[ImGuiCol_ButtonActive]           = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);

		colors[ImGuiCol_Header]                 = ImVec4(0.22f, 0.35f, 0.28f, 0.55f);
		colors[ImGuiCol_HeaderHovered]          = ImVec4(0.35f, 0.65f, 0.45f, 0.80f);
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);

		colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.30f, 0.32f, 0.50f);
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.35f, 0.65f, 0.45f, 0.78f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);

		colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.65f, 0.45f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.35f, 0.65f, 0.45f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.52f, 0.80f, 0.09f, 0.95f);

		colors[ImGuiCol_DockingPreview]         = ImVec4(0.35f, 0.65f, 0.45f, 0.70f);
		colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

		colors[ImGuiCol_PlotLines]              = ImVec4(0.35f, 0.65f, 0.45f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);
		colors[ImGuiCol_PlotHistogram]          = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.35f, 0.65f, 0.45f, 1.00f);

		colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.35f, 0.65f, 0.45f, 0.70f);
		colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

		colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.35f, 0.65f, 0.45f, 0.35f);
		colors[ImGuiCol_DragDropTarget]         = ImVec4(0.52f, 0.80f, 0.09f, 0.90f);
		//colors[ImGuiCol_NavHighlight]           = ImVec4(0.52f, 0.80f, 0.09f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
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