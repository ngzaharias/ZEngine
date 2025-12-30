#include "TextureEditorPCH.h"
#include "TextureEditor/TextureEditorWindowSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TextureHelpers.h"
#include "TextureEditor/TextureEditorAssetImportComponent.h"
#include "TextureEditor/TextureEditorAssetNewComponent.h"
#include "TextureEditor/TextureEditorAssetOpenComponent.h"
#include "TextureEditor/TextureEditorAssetSaveComponent.h"
#include "TextureEditor/TextureEditorSettingsSingleton.h"
#include "TextureEditor/TextureEditorWindowComponent.h"
#include "TextureEditor/TextureEditorWindowRequest.h"

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

	using World = editor::texture::WindowSystem::World;

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
					world.AddComponent<editor::texture::AssetNewComponent>(entity);

				if (ImGui::MenuItem("Import", "Ctrl + I"))
					world.AddComponent<editor::texture::AssetImportComponent>(entity);

				if (ImGui::MenuItem("Open", "Ctrl + O"))
					world.AddComponent<editor::texture::AssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save", "Ctrl + S"))
					world.AddComponent<editor::texture::AssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::texture::WindowComponent>(entity);
		eng::Texture2DAsset& texture = windowComponent.m_Asset;

		imgui::InputText("m_Guid", texture.m_Guid);
		imgui::InputText("m_Name", texture.m_Name);
	}

	void Asset_Import(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::texture::AssetImportComponent>(entity))
			world.RemoveComponent<editor::texture::AssetImportComponent>(entity);

		if (HasInput(world, strImport) || world.HasComponent<editor::texture::AssetImportComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::texture::SettingsSingleton>();

			eng::SelectFileSettings settings;
			settings.m_Title = "Import Texture";
			settings.m_Filters = { "PNG (*.png)", "*.png" };
			settings.m_Path = readSettings.m_Import;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::texture::SettingsSingleton>();
				writeSettings.m_Import = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::texture::WindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.ImportFromFile(writeWindow.m_Asset, filepath);
			}
		}
	};

	void Asset_New(World& world, const ecs::Entity& entity)
	{
		if (world.HasComponent<editor::texture::AssetNewComponent>(entity))
			world.RemoveComponent<editor::texture::AssetNewComponent>(entity);

		if (HasInput(world, strNew) || world.HasComponent<editor::texture::AssetNewComponent>(entity))
		{
			auto& windowComponent = world.WriteComponent<editor::texture::WindowComponent>(entity);
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

		if (world.HasComponent<editor::texture::AssetOpenComponent>(entity))
			world.RemoveComponent<editor::texture::AssetOpenComponent>(entity);

		if (HasInput(world, strOpen) || world.HasComponent<editor::texture::AssetOpenComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::texture::SettingsSingleton>();

			eng::SelectFileSettings settings;
			settings.m_Title = "Open Texture";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = readSettings.m_Open;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::texture::SettingsSingleton>();
				writeSettings.m_Open = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::texture::WindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				//assetManager.LoadAsset(writeWindow.m_Asset, filepath);
			}
		}
	};

	void Asset_Save(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::texture::AssetSaveComponent>(entity))
			world.RemoveComponent<editor::texture::AssetSaveComponent>(entity);

		if (HasInput(world, strSave) || world.HasComponent<editor::texture::AssetSaveComponent>(entity))
		{
			const auto& readSettings = world.ReadSingleton<editor::texture::SettingsSingleton>();
			const auto& readWindow = world.ReadComponent<editor::texture::WindowComponent>(entity);

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Texture";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::Path(
				readSettings.m_Save, 
				readWindow.m_Asset.m_Name, 
				eng::AssetManager::s_Extension);

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::texture::SettingsSingleton>();
				writeSettings.m_Save = str::Path(filepath.GetParent(), "\\");

				auto& writeWindow = world.WriteComponent<editor::texture::WindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveToFile(writeWindow.m_Asset, filepath);
			}
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		const auto& windowComponent = world.ReadComponent<editor::texture::WindowComponent>(entity);
		const eng::Texture2DAsset& texture = windowComponent.m_Asset;

		const Vector2f textureSize = Vector2f((float)texture.m_Width, (float)texture.m_Height);
		const Vector2f regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = eng::FitImageToRegion(textureSize, regionSize);

		imgui::Image(texture.m_TextureId, imageSize);
	};
}

void editor::texture::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<editor::texture::WindowComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::EditorUI;
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

	if (world.HasAny<ecs::query::Removed<editor::texture::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const auto& request : world.Events<editor::texture::WindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Texture Editor");

		auto& window = world.AddComponent<editor::texture::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Texture Editor", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##texture", identifier);
		window.m_PreviewerLabel = ToLabel("Previewer##texture", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::texture::WindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::texture::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::texture::WindowComponent>>())
	{
		auto& windowComponent = view.WriteRequired<editor::texture::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, view);

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
			DrawInspector(world, view);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_PreviewerLabel.c_str()))
			DrawPreviewer(world, view);
		ImGui::End();

		Asset_Import(world, view);
		Asset_New(world, view);
		Asset_Open(world, view);
		Asset_Save(world, view);

		if (!isOpen)
			world.DestroyEntity(view);
	}
}