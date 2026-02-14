#include "SpriteEditorPCH.h"
#include "SpriteEditor/SpriteEditorWindowSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TextureHelpers.h"
#include "Math/AABB.h"
#include "SpriteEditor/SpriteEditorAssetNewComponent.h"
#include "SpriteEditor/SpriteEditorAssetOpenComponent.h"
#include "SpriteEditor/SpriteEditorAssetSaveComponent.h"
#include "SpriteEditor/SpriteEditorBatchingComponent.h"
#include "SpriteEditor/SpriteEditorOpenWindowEvent.h"
#include "SpriteEditor/SpriteEditorSettingsSingleton.h"
#include "SpriteEditor/SpriteEditorWindowComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_graph.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	const str::Guid uuidShader = GUID("cbbb7d3ff44b45fdb9e5a207d92262fb");
	const str::Guid uuidStaticMesh = GUID("e94876a8e4cc4d1684c85859b48a1af6");
	const str::Guid uuidTexture2D = GUID("c6bb231ce97f104e860eb55e71988bdb");
	const str::Name strInput = str::Name::Create("SpriteEditor");
	const str::Name strNew = str::Name::Create("SpriteEditor_New");
	const str::Name strOpen = str::Name::Create("SpriteEditor_Open");
	const str::Name strSave = str::Name::Create("SpriteEditor_Save");

	using World = editor::sprite::WindowSystem::World;

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

	Vector2f ToPosition(const uint32 index, const editor::sprite::BatchingComponent& data)
	{
		const uint32 x = (index % data.m_Iterations.x);
		const uint32 y = (index / data.m_Iterations.x) % data.m_Iterations.y;
		return Vector2f(
			data.m_Initial.x + (data.m_Stride.x * x),
			data.m_Initial.y + (data.m_Stride.y * y));
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					world.AddComponent<editor::sprite::AssetNewComponent>(entity);

				if (ImGui::MenuItem("Open", "Ctrl+O"))
					world.AddComponent<editor::sprite::AssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					world.AddComponent<editor::sprite::AssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strNew))
			world.AddComponent<editor::sprite::AssetNewComponent>(entity);
		if (input.IsPressed(strOpen))
			world.AddComponent<editor::sprite::AssetOpenComponent>(entity);
		if (input.IsPressed(strSave))
			world.AddComponent<editor::sprite::AssetSaveComponent>(entity);
	};

	void DrawBatcher(World& world, const ecs::Entity& entity)
	{
		auto& batch = world.WriteComponent<editor::sprite::BatchingComponent>(entity);

		ImGui::TextDisabled("Display:");
		ImGui::Checkbox("m_IsPreviewing", &batch.m_IsPreviewing);

		ImGui::Separator();

		ImGui::TextDisabled("Options:");
		if (ImGui::Button("Extract"))
		{
			const auto& settings = world.ReadSingleton<editor::sprite::SettingsSingleton>();
			const auto& window = world.ReadComponent<editor::sprite::WindowComponent>(entity);

			eng::SelectFolderSettings fileSettings;
			fileSettings.m_Title = "Save Sprites";
			fileSettings.m_Path = settings.m_Extract;

			const str::Path folderpath = eng::SelectFolderDialog(fileSettings);
			if (!folderpath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::sprite::SettingsSingleton>();
				writeSettings.m_Extract = folderpath;

				auto& writeWindow = world.WriteComponent<editor::sprite::WindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();

				str::String name;
				str::Path filepath;
				eng::SpriteAsset& asset = writeWindow.m_Asset;

				const int32 digits = math::ToDigits(batch.m_Count);
				for (int32 i = 0; i < batch.m_Count; ++i)
				{
					name = batch.m_Format + str::ToString(i + 1, digits);
					filepath = str::Path(folderpath, "/", name);

					asset.m_Guid = str::Guid::Generate();
					asset.m_Name = str::Name::Create(name);
					asset.m_Position = ToPosition(i, batch);

					assetManager.SaveToFile(asset, filepath);
				}
			}
		}

		{
			const int32 countMax = batch.m_Iterations.x * batch.m_Iterations.y;
			imgui::DragVector("m_Iterations", batch.m_Iterations, 0.05f, 1, INT16_MAX);
			imgui::DragVector("m_Initial", batch.m_Initial);
			imgui::DragVector("m_Stride", batch.m_Stride);
			ImGui::SliderInt("m_Count", &batch.m_Count, 0, countMax);
		}

		ImGui::Separator();

		{
			ImGui::TextDisabled("Names:");
			imgui::InputText("Format", batch.m_Format);

			str::String name;
			const int32 digits = math::ToDigits(batch.m_Count);
			for (int32 i = 0; i < batch.m_Count; ++i)
			{
				// #todo: 
				//name = std::vformat(batch.m_Format, std::make_format_args(index));
				name = batch.m_Format + str::ToString(i + 1, digits);

				imgui::Text(name);
			}
		}
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<editor::sprite::WindowComponent>(entity);
		eng::SpriteAsset& sprite = window.m_Asset;

		str::Guid spritePrev = sprite.m_Guid;

		imgui::Inspector inspector;
		if (inspector.Begin("##inspector"))
		{
			const auto& assetManager = world.ReadResource<eng::AssetManager>();

			inspector.Write("m_Guid", sprite.m_Guid);
			inspector.Write("m_Name", sprite.m_Name);
			inspector.Write("m_Shader", sprite.m_Shader);
			imgui::WriteTexture2D(assetManager, "m_Texture2D", sprite.m_Texture2D);
			inspector.Write("m_Position", sprite.m_Position);
			inspector.Write("m_Size", sprite.m_Size);
			inspector.End();
		}

		if (ImGui::CollapsingHeader("Batcher"))
			DrawBatcher(world, entity);
	}

	void Asset_New(World& world, const ecs::Entity& entity)
	{
		if (world.HasComponent<editor::sprite::AssetNewComponent>(entity))
		{
			world.RemoveComponent<editor::sprite::AssetNewComponent>(entity);
			
			auto& window = world.WriteComponent<editor::sprite::WindowComponent>(entity);
			if (!window.m_Asset.m_Guid.IsValid())
			{
				window.m_Asset.m_Shader = uuidShader;
				window.m_Asset.m_Texture2D = uuidTexture2D;
				window.m_Asset.m_Size = Vector2f(128.f);
			}

			window.m_Asset.m_Guid = str::Guid::Generate();
			window.m_Asset.m_Name = str::Name::Create("SP_");
		}
	}

	void Asset_Open(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::sprite::AssetOpenComponent>(entity))
		{
			world.RemoveComponent<editor::sprite::AssetOpenComponent>(entity);

			const auto& settings = world.ReadSingleton<editor::sprite::SettingsSingleton>();

			eng::SelectFileSettings fileSettings;
			fileSettings.m_Title = "Open Sprite";
			fileSettings.m_Filters = { "Assets (*.asset)", "*.asset" };
			fileSettings.m_Path = settings.m_Open;

			const str::Path filepath = eng::SelectFileDialog(fileSettings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::sprite::SettingsSingleton>();
				writeSettings.m_Open = filepath.GetDirectory();

				auto& assetManager = world.WriteResource<eng::AssetManager>();
				auto& writeWindow = world.WriteComponent<editor::sprite::WindowComponent>(entity);
				assetManager.LoadFromFile(writeWindow.m_Asset, filepath);
			}
		}
	};

	void Asset_Save(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::sprite::AssetSaveComponent>(entity))
		{
			world.RemoveComponent<editor::sprite::AssetSaveComponent>(entity);

			const auto& settings = world.ReadSingleton<editor::sprite::SettingsSingleton>();
			const auto& window = world.ReadComponent<editor::sprite::WindowComponent>(entity);
			const str::Name& name = window.m_Asset.m_Name;

			eng::SaveFileSettings fileSettings;
			fileSettings.m_Title = "Save Sprite";
			fileSettings.m_Filters = { "Assets (*.asset)", "*.asset" };
			fileSettings.m_Path = str::Path(
				settings.m_Save, 
				window.m_Asset.m_Name, 
				eng::AssetManager::s_Extension);

			const str::Path filepath = eng::SaveFileDialog(fileSettings);
			if (!filepath.IsEmpty())
			{
				auto& writeSettings = world.WriteSingleton<editor::sprite::SettingsSingleton>();
				writeSettings.m_Save = filepath.GetDirectory();

				auto& writeWindow = world.WriteComponent<editor::sprite::WindowComponent>(entity);
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveToFile(writeWindow.m_Asset, filepath);
			}
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<editor::sprite::WindowComponent>(entity);
		eng::SpriteAsset& sprite = window.m_Asset;
		if (!sprite.m_Texture2D.IsValid())
			return;

		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.ReadAsset<eng::Texture2DAsset>(sprite.m_Texture2D);
		if (!textureAsset)
			return;

		const Vector2f spritePos = Vector2f((float)sprite.m_Position.x, (float)sprite.m_Position.y);
		const Vector2f spriteSize = Vector2f((float)sprite.m_Size.x, (float)sprite.m_Size.y);
		const Vector2f regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = eng::FitImageToRegion(spriteSize, regionSize);

		Vector2f uv0 = spritePos;
		Vector2f uv1 = spritePos + spriteSize;
		uv0.x = math::Remap(uv0.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
		uv0.y = math::Remap(uv0.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
		uv1.x = math::Remap(uv1.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
		uv1.y = math::Remap(uv1.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
		imgui::Image(textureAsset->m_TextureId, imageSize, uv0, uv1);
	};

	void DrawTexture(World& world, const ecs::Entity& entity)
	{
		auto& window = world.WriteComponent<editor::sprite::WindowComponent>(entity);
		eng::SpriteAsset& sprite = window.m_Asset;
		if (!sprite.m_Texture2D.IsValid())
			return;

		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.ReadAsset<eng::Texture2DAsset>(sprite.m_Texture2D);
		if (!textureAsset)
			return;

		const Vector2f textureSize = Vector2f((float)textureAsset->m_Width, (float)textureAsset->m_Height);
		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = eng::FitImageToRegion(textureSize, regionSize);
		const ImVec2 regionMin = ImGui::GetCursorScreenPos();
		const ImVec2 regionMax = ImVec2(regionMin.x + imageSize.x, regionMin.y + imageSize.y);

		imgui::Image(textureAsset->m_TextureId, imageSize);

		// draw rect
		{
			const Vector2f spritePos = Vector2f((float)sprite.m_Position.x, (float)sprite.m_Position.y);
			const Vector2f spriteSize = Vector2f((float)sprite.m_Size.x, (float)sprite.m_Size.y);

			Vector2f min = spritePos;
			Vector2f max = min + spriteSize;
			min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
			max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

			constexpr Colour colour = Colour(1.0f, 1.0f, 0.4f, 1.0f);
			imgui::AddRect({ min, max }, colour);
		}

		// draw sprites to be extracted
		const auto& batch = world.ReadComponent<editor::sprite::BatchingComponent>(entity);
		if (batch.m_IsPreviewing)
		{
			for (int32 i = 0; i < batch.m_Count; ++i)
			{
				Vector2f min = ToPosition(i, batch);
				Vector2f max = min + window.m_Asset.m_Size;
				min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
				min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
				max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
				max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

				imgui::AddRect({ min, max }, Vector4f(1.0f, 0.4f, 0.0f, 1.0f));
			}
		}
	};
}

void editor::sprite::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<editor::sprite::WindowComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::EditorUI;
		layer.m_Bindings.Emplace(strNew,  input::EKey::N, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strNew,  input::EKey::N, input::EKey::Control_R);
		layer.m_Bindings.Emplace(strOpen, input::EKey::O, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strOpen, input::EKey::O, input::EKey::Control_R);
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_R);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<editor::sprite::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const auto& request : world.Events<editor::sprite::OpenWindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Sprite Editor");
		world.AddComponent<editor::sprite::BatchingComponent>(windowEntity);

		auto& window = world.AddComponent<editor::sprite::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Sprite Editor##sprite", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##sprite", identifier);
		window.m_PreviewerLabel = ToLabel("Previewer##sprite", identifier);
		window.m_TextureLabel   = ToLabel("Texture##sprite", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::sprite::WindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::sprite::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::sprite::WindowComponent>>())
	{
		auto& window = view.WriteRequired<editor::sprite::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, view);

			const ImGuiID dockspaceId = ImGui::GetID(window.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID textureId, inspectorId, previewerId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &textureId, &inspectorId);
				ImGui::DockBuilderSplitNode(inspectorId, ImGuiDir_Up, 0.6f, &inspectorId, &previewerId);
				ImGui::DockBuilderDockWindow(window.m_TextureLabel.c_str(), textureId);
				ImGui::DockBuilderDockWindow(window.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(window.m_PreviewerLabel.c_str(), previewerId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(window.m_TextureLabel.c_str()))
			DrawTexture(world, view);
		ImGui::End();

		if (ImGui::Begin(window.m_InspectorLabel.c_str()))
			DrawInspector(world, view);
		ImGui::End();

		if (ImGui::Begin(window.m_PreviewerLabel.c_str()))
			DrawPreviewer(world, view);
		ImGui::End();

		Asset_New(world, view);
		Asset_Open(world, view);
		Asset_Save(world, view);

		if (!isOpen)
			world.DestroyEntity(view);

		if (window.m_Asset.m_Shader != window.m_ShaderLoaded)
		{
			auto& assetManager = world.WriteResource<eng::AssetManager>();
			if (assetManager.HasAsset(window.m_ShaderLoaded))
				assetManager.ReleaseAsset(window.m_ShaderLoaded);

			window.m_ShaderLoaded = window.m_Asset.m_Shader;

			if (assetManager.HasAsset(window.m_ShaderLoaded))
				assetManager.RequestAsset(window.m_ShaderLoaded);
		}

		if (window.m_Asset.m_Texture2D != window.m_TextureLoaded)
		{
			auto& assetManager = world.WriteResource<eng::AssetManager>();
			if (assetManager.HasAsset(window.m_TextureLoaded))
				assetManager.ReleaseAsset(window.m_TextureLoaded);

			window.m_TextureLoaded = window.m_Asset.m_Texture2D;

			if (assetManager.HasAsset(window.m_TextureLoaded))
				assetManager.RequestAsset(window.m_TextureLoaded);
		}
	}
}