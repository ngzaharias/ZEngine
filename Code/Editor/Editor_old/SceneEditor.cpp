#include "EditorPCH.h"
#include "Editor/SceneEditor.h"

#include "Editor/HistorianComponents.h"

#include <Engine/RenderSystem.h>
#include <Engine/Screen.h>
#include <Engine/TransformComponent.h>

#include <Engine/GLFW/Window.h>

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

// #todo: editor camera
// #todo: multiple cameras
// #todo: multiple windows
// #todo: dropdown select camera

editor::SceneEditor::SceneEditor(glfw::Window& window)
	: m_Window(window)
{
}

void editor::SceneEditor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;

	if (ImGui::Begin("Scene", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::EndMenuBar();
		}

		//sf::RenderTexture& renderTexture = m_SFMLWindow.m_Texture;

		//// #fixme: update the camera size
		//const sf::Vector2f regionSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
		//const sf::Vector2f viewSize = renderTexture.getView().getSize();
		//Screen::width = regionSize.x;
		//Screen::height = regionSize.y;

		//ImGui::Image(renderTexture.getTexture(), viewSize);
	}
	ImGui::End();
}