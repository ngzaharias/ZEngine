#include "EditorPCH.h"
#include "Editor/TextureEditor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Editor/TextureHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Texture2DAsset.h"

#include "GameDebug/MenuBarComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>

namespace
{
	using World = editor::TextureEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	bool HasInput_Import(World& world)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(entity);
			return input.IsKeyHeld(input::EKeyboard::Control_L) && input.IsKeyPressed(input::EKeyboard::I);
		}
		return false;
	}

	bool HasInput_Save(World& world)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(entity);
			return input.IsKeyHeld(input::EKeyboard::Control_L) && input.IsKeyPressed(input::EKeyboard::S);
		}
		return false;
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Import", "Ctrl + I"))
					world.AddComponent<editor::TextureAssetImportComponent>(entity);

				if (ImGui::MenuItem("Open"))
					world.AddComponent<editor::TextureAssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save", "Ctrl + S"))
					world.AddComponent<editor::TextureAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::TextureWindowComponent>(entity);
		eng::Texture2DAsset& texture = windowComponent.m_Asset;

		imgui::Guid("m_Guid", texture.m_Guid);
		imgui::Name("m_Name", texture.m_Name);
	}

	void DrawPopupImport(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetImportComponent>(entity))
			world.RemoveComponent<editor::TextureAssetImportComponent>(entity);

		if (world.HasComponent<editor::TextureAssetImportComponent>(entity) || HasInput_Import(world))
		{
			const auto& readSettings = world.ReadSingleton<edit::settings::LocalComponent>();
			const auto& readWindow = world.ReadComponent<editor::TextureWindowComponent>(entity);

			eng::SelectFileSettings settings;
			settings.m_Title = "Select File";
			settings.m_Filters.Append({ "PNG (*.png)", "*.png" });
			settings.m_Path = readSettings.m_Texture.m_Import;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<edit::settings::LocalComponent>();
				writeSettings.m_Texture.m_Import = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::TextureWindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ImportAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void DrawPopupOpen(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetOpenComponent>(entity))
		{
			ImGui::OpenPopup("Open Texture");

			world.RemoveComponent<editor::TextureAssetOpenComponent>(entity);
		}

		if (ImGui::BeginPopupModal("Open Texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("OK", ImVec2(120, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	};

	void DrawPopupSave(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetSaveComponent>(entity))
			world.RemoveComponent<editor::TextureAssetSaveComponent>(entity);

		if (world.HasComponent<editor::TextureAssetSaveComponent>(entity) || HasInput_Save(world))
		{
			const auto& readSettings = world.ReadSingleton<edit::settings::LocalComponent>();
			const auto& readWindow = world.ReadComponent<editor::TextureWindowComponent>(entity);
			const str::Name& name = readWindow.m_Asset.m_Name;

			eng::SaveFileSettings settings;
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::Path(readSettings.m_Texture.m_Save, name);

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<edit::settings::LocalComponent>();
				writeSettings.m_Texture.m_Save = str::Path(filepath.GetParent(), "\\");

				auto& writeWindow = world.WriteComponent<editor::TextureWindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::TextureWindowComponent>(entity);
		eng::Texture2DAsset& texture = windowComponent.m_Asset;

		const Vector2f textureSize = Vector2f((float)texture.m_Width, (float)texture.m_Height);
		const Vector2f regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = editor::FitImageToRegion(textureSize, regionSize);

		imgui::Image(texture.m_TextureId, imageSize);
	};
}

void editor::TextureEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::TextureWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::TextureWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Texture Editor", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
		windowComponent.m_PreviewerLabel = ToLabel("Previewer", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::TextureWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::TextureWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID inspectorId, previewerId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &previewerId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_PreviewerLabel.c_str(), previewerId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_PreviewerLabel.c_str()))
			DrawPreviewer(world, windowEntity);
		ImGui::End();

		DrawPopupImport(world, windowEntity);
		DrawPopupOpen(world, windowEntity);
		DrawPopupSave(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}