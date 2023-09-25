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
	using World = editor::FlipbookEditor::World;

	constexpr float s_TimeMaxEpsilon = 0.000001f;

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

	Vector2f ToPosition(const uint32 index, const editor::FlipbookExtractorComponent& data)
	{
		const uint32 x = (index % data.m_Dimensions.x);
		const uint32 y = (index / data.m_Dimensions.x) % data.m_Dimensions.y;
		return Vector2f(
			data.m_Initial.x + (data.m_Stride.x * x), 
			data.m_Initial.y + (data.m_Stride.y * y));
	}

	void DrawExtractor(World& world, const ecs::Entity& entity)
	{
		auto& extractorComponent = world.GetComponent<editor::FlipbookExtractorComponent>(entity);

		if (ImGui::Button("Extract"))
		{
			auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);
			for (int32 i = 0; i < extractorComponent.m_Count; ++i)
			{
				eng::FlipbookFrame& frame = windowComponent.m_Asset.m_Frames.Emplace();
				frame.m_Position = ToPosition(i, extractorComponent);
				frame.m_Size = extractorComponent.m_Size;
			}
		}

		ImGui::Separator();

		ImGui::TextDisabled("Batch:");
		imgui::DragUInt2("m_Dimensions", &extractorComponent.m_Dimensions.x, 0.05f, 1, INT16_MAX);
		ImGui::DragFloat2("m_Initial", &extractorComponent.m_Initial.x);
		ImGui::DragFloat2("m_Stride", &extractorComponent.m_Stride.x);

		ImGui::Separator();

		ImGui::TextDisabled("Sprite:");
		const int32 countMax = extractorComponent.m_Dimensions.x * extractorComponent.m_Dimensions.y;
		ImGui::DragFloat2("m_Size", &extractorComponent.m_Size.x);
		ImGui::SliderInt("m_Count", &extractorComponent.m_Count, 0, countMax);

		ImGui::Separator();

		ImGui::TextDisabled("Display:");
		ImGui::Checkbox("m_IsPreviewing", &extractorComponent.m_IsPreviewing);
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;

		imgui::Guid("m_Guid", flipbook.m_Guid);
		imgui::Name("m_Name", flipbook.m_Name);
		imgui::Path("m_Path", flipbook.m_Path);

		ImGui::Separator();

		imgui::Guid("m_Shader", flipbook.m_Shader);
		imgui::Guid("m_StaticMesh", flipbook.m_StaticMesh);
		imgui::Guid("m_Texture2D", flipbook.m_Texture2D);

		ImGui::DragFloat("m_FPS", &flipbook.m_FPS);
		ImGui::Checkbox("m_IsLooping", &flipbook.m_IsLooping);

		if (ImGui::CollapsingHeader("m_Frames", ImGuiTreeNodeFlags_DefaultOpen))
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
					ImGui::DragFloat2("m_Position", &frame.m_Position.x);
					ImGui::DragFloat2("m_Size", &frame.m_Size.x);
					ImGui::Unindent();
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					eng::FlipbookAsset flipbook;
					flipbook.m_Guid = str::Guid::Generate();
					flipbook.m_Name = str::Name::Create("FP_MyFlipbook");
					flipbook.m_Shader = uuidShader;
					flipbook.m_StaticMesh = uuidStaticMesh;
					flipbook.m_Texture2D = str::Guid::Create("f9dcfdd6-014a-c528-12e2-1e73f232b7f9");

					auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);
					windowComponent.m_Asset = flipbook;
				}

				if (ImGui::MenuItem("Open"))
					world.AddComponent<editor::FlipbookAssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save"))
					world.AddComponent<editor::FlipbookAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawPopupOpen(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::FlipbookAssetOpenComponent>(entity))
		{
			world.RemoveComponent<editor::FlipbookAssetOpenComponent>(entity);
		}
	};

	void DrawPopupSave(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::FlipbookAssetSaveComponent>(entity))
		{
			auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Flipbook";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Directory = str::GetPath(str::EPath::Assets);
			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& assetBrowser = world.GetManager<eng::AssetManager>();
				assetBrowser.SaveAsset(windowComponent.m_Asset, filepath);
			}

			world.RemoveComponent<editor::FlipbookAssetSaveComponent>(entity);
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;
		if (!flipbook.m_Texture2D.IsValid())
			return;

		auto& assetBrowser = world.GetManager<eng::AssetManager>();
		const auto* textureAsset = assetBrowser.LoadAsset<eng::Texture2DAsset>(flipbook.m_Texture2D);
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
			Vector2f uv0 = frame.m_Position;
			Vector2f uv1 = frame.m_Position + frame.m_Size;
			uv0.x = math::Remap(uv0.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
			uv0.y = math::Remap(uv0.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
			uv1.x = math::Remap(uv1.x, 0.f, (float)textureAsset->m_Width, 0.f, 1.f);
			uv1.y = math::Remap(uv1.y, 0.f, (float)textureAsset->m_Height, 0.f, 1.f);
			imgui::Image(textureAsset->m_TextureId, imageSize, uv0, uv1);
		}
	};

	void DrawTexture(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;
		if (!flipbook.m_Texture2D.IsValid())
			return;

		auto& assetBrowser = world.GetManager<eng::AssetManager>();
		const auto* textureAsset = assetBrowser.LoadAsset<eng::Texture2DAsset>(flipbook.m_Texture2D);
		if (!textureAsset)
			return;

		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const ImVec2 regionMin = ImGui::GetCursorScreenPos();
		const ImVec2 regionMax = ImVec2(regionMin.x + regionSize.x, regionMin.y + regionSize.y);

		const ImTextureID textureId = (void*)(intptr_t)textureAsset->m_TextureId;
		ImGui::Image(textureId, regionSize, { 0, 1 }, { 1, 0 });

		// draw frames of the flipbook
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (const eng::FlipbookFrame& frame : flipbook.m_Frames)
		{
			Vector2f min = frame.m_Position;
			Vector2f max = min + frame.m_Size;
			min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
			max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

			imgui::AddRect(min, max, Vector4f(1.0f, 1.0f, 0.4f, 1.0f));
		}

		// draw frames to be extracted
		const auto& extractorComponent = world.GetComponent<const editor::FlipbookExtractorComponent>(entity);
		if (extractorComponent.m_IsPreviewing)
		{
			for (int32 i = 0; i < extractorComponent.m_Count; ++i)
			{
				Vector2f min = ToPosition(i, extractorComponent);
				Vector2f max = min + extractorComponent.m_Size;
				min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
				min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
				max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
				max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

				imgui::AddRect(min, max, Vector4f(1.0f, 0.4f, 0.0f, 1.0f));
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
		world.AddComponent<editor::FlipbookExtractorComponent>(windowEntity);

		auto& windowComponent = world.AddComponent<editor::FlipbookWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Flipbook Editor", windowEntity);
		windowComponent.m_ExtractorLabel = ToLabel("Extractor", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
		windowComponent.m_PreviewerLabel = ToLabel("Previewer", windowEntity);
		windowComponent.m_TextureLabel   = ToLabel("Texture", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::FlipbookWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<editor::FlipbookWindowComponent>(windowEntity);

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
				ImGui::DockBuilderDockWindow(windowComponent.m_ExtractorLabel.c_str(), inspectorId);
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

		if (ImGui::Begin(windowComponent.m_ExtractorLabel.c_str()))
			DrawExtractor(world, windowEntity);
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