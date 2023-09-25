#include "EditorPCH.h"
#include "Editor/FlipbookEditor.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/Texture2DAsset.h>

#include <GameDebug/MenuBarComponents.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;

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

	using World = editor::FlipbookEditor::World;
	void DrawExtractor(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		if (!windowComponent.m_Asset)
			return;

		eng::FlipbookAsset& flipbook = *windowComponent.m_Asset;
		if (!flipbook.m_Texture.IsValid())
			return;

		auto& assetBrowser = world.GetManager<eng::AssetManager>();
		const auto* textureAsset = assetBrowser.LoadAsset<eng::Texture2DAsset>(flipbook.m_Texture);
		if (!textureAsset)
			return;

		const ImVec2 rangeMin = ImGui::GetCursorScreenPos();
		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const ImTextureID textureId = (void*)(intptr_t)textureAsset->m_TextureId;
		ImGui::Image(textureId, regionSize, { 0, 1 }, { 1, 0 });

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		const Vector2f rangeMax = Vector2f(rangeMin.x + regionSize.x, rangeMin.y + regionSize.y);
		for (const eng::FlipbookFrame& frame : flipbook.m_Frames)
		{
			Vector2f min = Vector2f( (float)frame.m_Min.x, (float)frame.m_Min.y );
			Vector2f max = Vector2f( (float)frame.m_Max.x, (float)frame.m_Max.y );
			min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, rangeMin.x, rangeMax.x);
			min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, rangeMax.y, rangeMin.y);
			max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, rangeMin.x, rangeMax.x);
			max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, rangeMax.y, rangeMin.y);

			imgui::AddRect(min, max, Vector4f(1.0f, 1.0f, 0.4f, 1.0f));
		}
	};

	void DrawInspector(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		if (!windowComponent.m_Asset)
			return;

		eng::FlipbookAsset& flipbook = *windowComponent.m_Asset;

		str::String texture = flipbook.m_Texture.ToString();
		if (ImGui::InputText("Texture", &texture) && str::Guid::IsValidString(texture))
			flipbook.m_Texture = str::Guid::Create(texture);

		ImGui::DragFloat("FPS", &flipbook.m_FPS);
		ImGui::Checkbox("Loop", &flipbook.m_IsLooping);

		if (ImGui::CollapsingHeader("Frames", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();
			if (ImGui::Button("Add"))
				flipbook.m_Frames.Emplace();
			ImGui::SameLine();
			if (ImGui::Button("Pop"))
				flipbook.m_Frames.Pop();
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
				flipbook.m_Frames.RemoveAll();

			for (auto&& [i, frame] : enumerate::Forward(flipbook.m_Frames))
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Frame", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent();
					imgui::DragUInt2("Min", &frame.m_Min.x);
					imgui::DragUInt2("Max", &frame.m_Max.x);
					ImGui::Unindent();
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
	}

	void DrawMenuBar(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
					windowComponent.m_WasNewRequested = true;
				if (ImGui::MenuItem("Open..."))
					windowComponent.m_WasOpenRequested = true;
				if (ImGui::MenuItem("Save"))
					windowComponent.m_WasSaveRequested = true;
				if (ImGui::MenuItem("Save As..."))
					windowComponent.m_WasSaveAsRequested = true;

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawOpenAsset(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (windowComponent.m_WasOpenRequested)
			ImGui::OpenPopup(windowComponent.m_PopupOpenLabel.c_str(), s_PopupFlags);

		bool isOpen = true;
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal(windowComponent.m_PopupOpenLabel.c_str(), &isOpen, s_WindowFlags))
		{
			ImGui::EndPopup();
		}
	};

	void DrawPreviewer(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		if (!windowComponent.m_Asset)
			return;

		eng::FlipbookAsset& flipbook = *windowComponent.m_Asset;
		if (!flipbook.m_Texture.IsValid())
			return;

		auto& assetBrowser = world.GetManager<eng::AssetManager>();
		const auto* textureAsset = assetBrowser.LoadAsset<eng::Texture2DAsset>(flipbook.m_Texture);
		if (!textureAsset)
			return;

		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = Vector2f((float)regionSize.x, (float)regionSize.y);

		if (flipbook.m_Frames.IsEmpty())
		{
			imgui::Image(textureAsset->m_TextureId, imageSize);
		}
		else
		{
			const eng::FlipbookFrame& frame = flipbook.m_Frames.GetLast();
			Vector2f uv0 = Vector2f((float)frame.m_Min.x, (float)frame.m_Min.y);
			Vector2f uv1 = Vector2f((float)frame.m_Max.x, (float)frame.m_Max.y);
			uv0.x = math::Remap(uv0.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
			uv0.y = math::Remap(uv0.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
			uv1.x = math::Remap(uv1.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
			uv1.y = math::Remap(uv1.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
			imgui::Image(textureAsset->m_TextureId, imageSize, uv0, uv1);
		}
	};

	void DrawSaveAsset(World& world, editor::FlipbookWindowComponent& windowComponent)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (windowComponent.m_WasSaveAsRequested)
		{
			eng::SaveFileSettings settings;
			settings.m_Title = "Save Flipbook";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Directory = str::GetPath(str::EPath::Assets);
			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty() && windowComponent.m_Asset)
			{
				eng::FlipbookAsset& flipbook = *windowComponent.m_Asset;
				auto& assetBrowser = world.GetManager<eng::AssetManager>();
				assetBrowser.SaveAsset(flipbook, filepath);
			}
		}
	};
}

void editor::FlipbookEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::FlipbookWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::FlipbookWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Flipbook Editor", windowEntity);
		windowComponent.m_ExtractorLabel = ToLabel("Extractor", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
		windowComponent.m_PreviewerLabel = ToLabel("Previewer", windowEntity);
		windowComponent.m_PopupOpenLabel = ToLabel("Asset: Open", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::FlipbookWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(windowEntity);
		windowComponent.m_WasNewRequested = false;
		windowComponent.m_WasOpenRequested = false;
		windowComponent.m_WasSaveAsRequested = false;
		windowComponent.m_WasSaveRequested = false;

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowComponent);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID extractorId, inspectorId, previewerId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &extractorId, &inspectorId);
				ImGui::DockBuilderSplitNode(inspectorId, ImGuiDir_Up, 0.6f, &inspectorId, &previewerId);
				ImGui::DockBuilderDockWindow(windowComponent.m_ExtractorLabel.c_str(), extractorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_PreviewerLabel.c_str(), previewerId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_ExtractorLabel.c_str()))
			DrawExtractor(world, windowComponent);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowComponent);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_PreviewerLabel.c_str()))
			DrawPreviewer(world, windowComponent);
		ImGui::End();

		if (windowComponent.m_WasNewRequested)
		{
			eng::FlipbookAsset flipbook;
			flipbook.m_Texture = str::Guid::Create("f87d23dd-5e7b-4d6d-bff8-8b0eb676f80c");
			flipbook.m_Frames.Emplace();
			windowComponent.m_Asset = flipbook;
		}

		DrawOpenAsset(world, windowComponent);
		DrawSaveAsset(world, windowComponent);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}