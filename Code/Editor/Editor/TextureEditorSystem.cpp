#include "EditorPCH.h"
#include "Editor/TextureEditorSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Editor/TextureHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/Texture2DAsset.h"

#include "GameDebug/MenuBarComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"

namespace
{
	const str::Name strInput = str::Name::Create("TextureEditor");
	const str::Name strImport = str::Name::Create("TextureEditor_Import");
	const str::Name strNew = str::Name::Create("TextureEditor_New");
	const str::Name strOpen = str::Name::Create("TextureEditor_Open");
	const str::Name strSave = str::Name::Create("TextureEditor_Save");

	using World = editor::TextureEditorSystem::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	bool HasInput(World& world, const str::Name& name)
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		return input.IsPressed(name);
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl + N"))
					world.AddComponent<editor::TextureAssetNewComponent>(entity);

				if (ImGui::MenuItem("Import", "Ctrl + I"))
					world.AddComponent<editor::TextureAssetImportComponent>(entity);

				if (ImGui::MenuItem("Open", "Ctrl + O"))
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

		imgui::InputText("m_Guid", texture.m_Guid);
		imgui::InputText("m_Name", texture.m_Name);
	}

	void Asset_Import(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetImportComponent>(entity))
			world.RemoveComponent<editor::TextureAssetImportComponent>(entity);

		if (HasInput(world, strImport) || world.HasComponent<editor::TextureAssetImportComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::settings::LocalComponent>();

			eng::SelectFileSettings settings;
			settings.m_Title = "Import Texture";
			settings.m_Filters = { "PNG (*.png)", "*.png" };
			settings.m_Path = readSettings.m_Texture.m_Import;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::settings::LocalComponent>();
				writeSettings.m_Texture.m_Import = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::TextureWindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ImportAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void Asset_New(World& world, const ecs::Entity& entity)
	{
		if (world.HasComponent<editor::TextureAssetNewComponent>(entity))
			world.RemoveComponent<editor::TextureAssetNewComponent>(entity);

		if (HasInput(world, strNew) || world.HasComponent<editor::TextureAssetNewComponent>(entity))
		{
			auto& windowComponent = world.WriteComponent<editor::TextureWindowComponent>(entity);
			windowComponent.m_Asset = {};

			eng::Texture2DAsset& sprite = windowComponent.m_Asset;
			sprite.m_Guid = str::Guid::Generate();
			sprite.m_Name = str::Name::Create("T_Texture");
		}
	}

	void Asset_Open(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetOpenComponent>(entity))
			world.RemoveComponent<editor::TextureAssetOpenComponent>(entity);

		if (HasInput(world, strOpen) || world.HasComponent<editor::TextureAssetOpenComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::settings::LocalComponent>();

			eng::SelectFileSettings settings;
			settings.m_Title = "Open Texture";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = readSettings.m_Texture.m_Open;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::settings::LocalComponent>();
				writeSettings.m_Texture.m_Open = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::TextureWindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.LoadAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void Asset_Save(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TextureAssetSaveComponent>(entity))
			world.RemoveComponent<editor::TextureAssetSaveComponent>(entity);

		if (HasInput(world, strSave) || world.HasComponent<editor::TextureAssetSaveComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::settings::LocalComponent>();
			const auto& readWindow = world.ReadComponent<editor::TextureWindowComponent>(entity);

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Texture";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::Path(
				readSettings.m_Texture.m_Save, 
				readWindow.m_Asset.m_Name, 
				eng::AssetManager::s_Extension);

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::settings::LocalComponent>();
				writeSettings.m_Texture.m_Save = str::Path(filepath.GetParent(), "\\");

				auto& writeWindow = world.WriteComponent<editor::TextureWindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		const auto& windowComponent = world.ReadComponent<editor::TextureWindowComponent>(entity);
		const eng::Texture2DAsset& texture = windowComponent.m_Asset;

		const Vector2f textureSize = Vector2f((float)texture.m_Width, (float)texture.m_Height);
		const Vector2f regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = editor::FitImageToRegion(textureSize, regionSize);

		imgui::Image(texture.m_TextureId, imageSize);
	};
}

void editor::TextureEditorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const int32 count = world.Query<ecs::query::Include<editor::TextureWindowComponent>>().GetCount();
	if (count == 1 && world.HasAny<ecs::query::Added<editor::TextureWindowComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Editor;
		layer.m_Bindings.Emplace(strImport, input::EKey::I, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strImport, input::EKey::I, input::EKey::Control_R);
		layer.m_Bindings.Emplace(strNew,    input::EKey::N, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strNew,    input::EKey::N, input::EKey::Control_R);
		layer.m_Bindings.Emplace(strOpen,   input::EKey::O, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strOpen,   input::EKey::O, input::EKey::Control_R);
		layer.m_Bindings.Emplace(strSave,   input::EKey::S, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSave,   input::EKey::S, input::EKey::Control_R);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (count == 0 && world.HasAny<ecs::query::Removed<editor::TextureWindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::TextureWindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Texture Editor");

		auto& window = world.AddComponent<editor::TextureWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Texture Editor", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##texture", identifier);
		window.m_PreviewerLabel = ToLabel("Previewer##texture", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::TextureWindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::TextureWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::TextureWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::TextureWindowComponent>(windowEntity);

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
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

		Asset_Import(world, windowEntity);
		Asset_New(world, windowEntity);
		Asset_Open(world, windowEntity);
		Asset_Save(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}