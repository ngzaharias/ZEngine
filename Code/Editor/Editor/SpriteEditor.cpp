#include "EditorPCH.h"
#include "Editor/SpriteEditor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/TextureHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Texture2DAsset.h"
#include "GameDebug/MenuBarComponents.h"
#include "Math/AABB.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>
#include <imgui/Inspector.h>

namespace
{
	using World = editor::SpriteEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	const str::Guid uuidShader = GUID("cbbb7d3f-f44b-45fd-b9e5-a207d92262fb");
	const str::Guid uuidStaticMesh = GUID("e94876a8-e4cc-4d16-84c8-5859b48a1af6");
	const str::Guid uuidTexture2D = GUID("c6bb231c-e97f-104e-860e-b55e71988bdb");

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
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
				if (ImGui::MenuItem("New"))
				{
					auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(entity);
					windowComponent.m_Asset = {};

					eng::SpriteAsset& sprite = windowComponent.m_Asset;
					sprite.m_Name = str::Name::Create("SP_Sprite");
					sprite.m_Shader = uuidShader;
					sprite.m_Texture2D = uuidTexture2D;
					sprite.m_Size = Vector2u(128);
				}

				if (ImGui::MenuItem("Open"))
					world.AddComponent<editor::SpriteAssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save") || HasInput_Save(world))
					world.AddComponent<editor::SpriteAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(entity);
		eng::SpriteAsset& sprite = windowComponent.m_Asset;

		imgui::Inspector inspector;
		inspector.Read("m_Guid", sprite.m_Guid);
		inspector.Read("m_Name", sprite.m_Name);

		ImGui::Separator();

		inspector.Read("m_Shader", sprite.m_Shader);
		inspector.Read("m_Texture2D", sprite.m_Texture2D);

		ImGui::Separator();

		inspector.Read("m_Position", sprite.m_Position);
		inspector.Read("m_Size", sprite.m_Size);
	}

	void DrawPopupOpen(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::SpriteAssetOpenComponent>(entity))
		{
			ImGui::OpenPopup("Open Sprite");

			world.RemoveComponent<editor::SpriteAssetOpenComponent>(entity);
		}

		if (ImGui::BeginPopupModal("Open Sprite", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

		if (world.HasComponent<editor::SpriteAssetSaveComponent>(entity))
		{
			auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(entity);
			const str::Name& name = windowComponent.m_Asset.m_Name;

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Sprite";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			//settings.m_Path = str::GetPath(str::EPath::Assets);
			settings.m_Path = name;

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveAsset(windowComponent.m_Asset, filepath);
			}

			world.RemoveComponent<editor::SpriteAssetSaveComponent>(entity);
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(entity);
		eng::SpriteAsset& sprite = windowComponent.m_Asset;
		if (!sprite.m_Texture2D.IsValid())
			return;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.LoadAsset<eng::Texture2DAsset>(sprite.m_Texture2D);
		if (!textureAsset)
			return;

		const Vector2f spritePos = Vector2f((float)sprite.m_Position.x, (float)sprite.m_Position.y);
		const Vector2f spriteSize = Vector2f((float)sprite.m_Size.x, (float)sprite.m_Size.y);
		const Vector2f regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = editor::FitImageToRegion(spriteSize, regionSize);

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
		auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(entity);
		eng::SpriteAsset& sprite = windowComponent.m_Asset;
		if (!sprite.m_Texture2D.IsValid())
			return;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.LoadAsset<eng::Texture2DAsset>(sprite.m_Texture2D);
		if (!textureAsset)
			return;

		const Vector2f textureSize = Vector2f((float)textureAsset->m_Width, (float)textureAsset->m_Height);
		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = editor::FitImageToRegion(textureSize, regionSize);

		// draw rect
		{
			const Vector2f spritePos = Vector2f((float)sprite.m_Position.x, (float)sprite.m_Position.y);
			const Vector2f spriteSize = Vector2f((float)sprite.m_Size.x, (float)sprite.m_Size.y);

			const ImVec2 regionMin = ImGui::GetCursorScreenPos();
			const ImVec2 regionMax = ImVec2(regionMin.x + imageSize.x, regionMin.y + imageSize.y);

			Vector2f min = spritePos;
			Vector2f max = min + spriteSize;
			min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
			max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

			imgui::AddRect({ min, max }, Vector4f(1.0f, 1.0f, 0.4f, 1.0f));
		}

		imgui::Image(textureAsset->m_TextureId, imageSize);
	};
}

void editor::SpriteEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::SpriteWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::SpriteWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Sprite Editor", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
		windowComponent.m_PreviewerLabel = ToLabel("Previewer", windowEntity);
		windowComponent.m_TextureLabel   = ToLabel("Texture", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::SpriteWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::SpriteWindowComponent>(windowEntity);

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

				ImGuiID textureId, inspectorId, previewerId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &textureId, &inspectorId);
				ImGui::DockBuilderSplitNode(inspectorId, ImGuiDir_Up, 0.6f, &inspectorId, &previewerId);
				ImGui::DockBuilderDockWindow(windowComponent.m_TextureLabel.c_str(), textureId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_PreviewerLabel.c_str(), previewerId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_TextureLabel.c_str()))
			DrawTexture(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_PreviewerLabel.c_str()))
			DrawPreviewer(world, windowEntity);
		ImGui::End();

		DrawPopupOpen(world, windowEntity);
		DrawPopupSave(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}