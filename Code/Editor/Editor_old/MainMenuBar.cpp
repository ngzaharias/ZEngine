#include "EditorPCH.h"
#include "Editor/MainMenuBar.h"

#include "Editor/AssetBrowser.h"
#include "Editor/EntityBrowser.h"
#include "Editor/Historian.h"
#include "Editor/Inspector.h"
#include "Editor/NodeGraphEditor.h"
#include "Editor/TemplateEditor.h"

#include <Engine/SoundSystem.h>

#include <imgui/imgui.h>

namespace
{
	const ImVec4 s_Black = ImVec4(0.f, 0.f, 0.f, 1.f);
	const ImVec4 s_Gray = ImVec4(0.09f, 0.09f, 0.09f, 1.f);
	const ImVec4 s_White = ImVec4(1.f, 1.f, 1.f, 1.f);

	const ImVec4 s_PurpleSelected = ImVec4(0.68f, 0.55f, 0.96f, 1.f);
	const ImVec4 s_PurpleHovered = ImVec4(0.55f, 0.36f, 0.95f, 1.f);
	const ImVec4 s_PurpleEnabled = ImVec4(0.37f, 0.23f, 0.69f, 1.f);
	const ImVec4 s_PurpleDisabled = ImVec4(0.27f, 0.19f, 0.46f, 1.f);

	const ImVec4 s_Valid = ImVec4(0.71f, 0.92f, 0.14f, 1.f);
	const ImVec4 s_Invalid = ImVec4(0.92f, 0.14f, 0.25f, 1.f);
}

editor::MainMenuBar::MainMenuBar(
	audio::SoundSystem& soundSystem,
	editor::AssetBrowser& assetBrowser,
	editor::EntityBrowser& entityBrowser,
	editor::Historian& historian,
	editor::Inspector& inspector,
	editor::NodeGraphEditor& nodeGraphEditor,
	editor::TemplateEditor& templateEditor)
	: m_SoundSystem(soundSystem)
	, m_AssetBrowser(assetBrowser)
	, m_EntityBrowser(entityBrowser)
	, m_Historian(historian)
	, m_Inspector(inspector)
	, m_NodeGraphEditor(nodeGraphEditor)
	, m_TemplateEditor(templateEditor)
{
}

void editor::MainMenuBar::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Render();
}

void editor::MainMenuBar::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Level...", "Ctrl+N", false, false);
			ImGui::MenuItem("Open Level...", "Ctrl+O", false, false);
			ImGui::MenuItem("Save Level", "Ctrl+S", false, false);
			ImGui::MenuItem("Save Level As...", "Ctrl+Shift+S", false, false);
			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::TextDisabled("History");
			ImGui::MenuItem("Undo\t", "Ctrl+Z", false, false);
			ImGui::MenuItem("Redo\t", "Ctrl+Y", false, false);
			ImGui::Separator();

			ImGui::TextDisabled("Edit");
			ImGui::MenuItem("Cut\t", "Ctrl+X", false, false);
			ImGui::MenuItem("Copy\t", "Ctrl+C", false, false);
			ImGui::MenuItem("Paste\t", "Ctrl+V", false, false);
			ImGui::MenuItem("Duplicate\t", "Ctrl+D", false, false);
			ImGui::MenuItem("Delete\t", "Delete", false, false);
			ImGui::Separator();

			ImGui::TextDisabled("Configuration");
			ImGui::MenuItem("Settings", nullptr, false, false);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Audio"))
		{
			float volume = m_SoundSystem.GetVolume();
			ImGui::SliderFloat("Volume", &volume, 0.f, 100.f, "%.0f");
			m_SoundSystem.SetVolume(volume);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("Profiler"))
			{
				// #todo: use relative path
				str::String currentPath = "start ";
				currentPath += std::filesystem::current_path().string();
				currentPath += "\\3rdParty\\optick\\1.3.1\\Optick.exe";
				system(currentPath.c_str());
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("General", nullptr, false, false);

			bool assetBrowser = m_AssetBrowser.IsVisible();
			if (ImGui::MenuItem("Asset Browser", nullptr, &assetBrowser))
				m_AssetBrowser.SetVisible(assetBrowser);
			bool entityBrowser = m_EntityBrowser.IsVisible();
			if (ImGui::MenuItem("Entity Browser", nullptr, &entityBrowser))
				m_EntityBrowser.SetVisible(entityBrowser);
			bool historian = m_Historian.IsVisible();
			if (ImGui::MenuItem("Historian", nullptr, &historian))
				m_Historian.SetVisible(historian);
			bool inspector = m_Inspector.IsVisible();
			if (ImGui::MenuItem("Inspector", nullptr, &inspector))
				m_Inspector.SetVisible(inspector);
			bool nodeGraphEditor = m_NodeGraphEditor.IsVisible();
			if (ImGui::MenuItem("Node Graph Editor", nullptr, &nodeGraphEditor))
				m_NodeGraphEditor.SetVisible(nodeGraphEditor);
			bool templateEditor = m_TemplateEditor.IsVisible();
			if (ImGui::MenuItem("Template Editor", nullptr, &templateEditor))
				m_TemplateEditor.SetVisible(templateEditor);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}