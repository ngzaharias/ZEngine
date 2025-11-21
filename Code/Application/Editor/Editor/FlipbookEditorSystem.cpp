#include "EditorPCH.h"
#include "Editor/FlipbookEditorSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/Texture2DAsset.h"
#include "GameDebug/MenuBarComponents.h"
#include "Math/AABB.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"

namespace
{
	using World = editor::FlipbookEditorSystem::World;

	constexpr float s_TimeMaxEpsilon = 0.000001f;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags = 
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	const str::Guid uuidShader = GUID("cbbb7d3ff44b45fdb9e5a207d92262fb");
	const str::Guid uuidTexture2D = GUID("c6bb231ce97f104e860eb55e71988bdb");

	Vector2f ToImageSize(const ImVec2& regionSize, const eng::Texture2DAsset& texture)
	{
		const float ratio = std::min(regionSize.x / (float)texture.m_Width, regionSize.y / (float)texture.m_Height);
		return Vector2f((float)texture.m_Width, (float)texture.m_Height) * ratio;
	}

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	Vector2f ToPosition(const uint32 index, const editor::FlipbookBatchingComponent& data)
	{
		const uint32 x = (index % data.m_Iterations.x);
		const uint32 y = (index / data.m_Iterations.x) % data.m_Iterations.y;
		return Vector2f(
			data.m_Initial.x + (data.m_Stride.x * x), 
			data.m_Initial.y + (data.m_Stride.y * y));
	}

	void DrawBatcher(World& world, const ecs::Entity& entity)
	{
		auto& extractorComponent = world.WriteComponent<editor::FlipbookBatchingComponent>(entity);

		if (ImGui::Button("Extract"))
		{
			auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
			for (int32 i = 0; i < extractorComponent.m_Count; ++i)
			{
				eng::FlipbookFrame& frame = windowComponent.m_Asset.m_Frames.Emplace();
				frame.m_Position = ToPosition(i, extractorComponent);
				frame.m_Size = extractorComponent.m_Size;
			}
		}

		ImGui::Separator();

		ImGui::TextDisabled("Batch:");
		imgui::DragVector("m_Iterations", extractorComponent.m_Iterations, 0.05f, 1, INT16_MAX);
		imgui::DragVector("m_Initial", extractorComponent.m_Initial);
		imgui::DragVector("m_Stride", extractorComponent.m_Stride);

		ImGui::Separator();

		ImGui::TextDisabled("Frames:");
		ImGui::DragFloat2("m_Size", &extractorComponent.m_Size.x);
		const int32 countMax = extractorComponent.m_Iterations.x * extractorComponent.m_Iterations.y;
		ImGui::SliderInt("m_Count", &extractorComponent.m_Count, 0, countMax);

		ImGui::Separator();

		ImGui::TextDisabled("Display:");
		ImGui::Checkbox("m_IsPreviewing", &extractorComponent.m_IsPreviewing);
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;

		imgui::InputText("m_Guid", flipbook.m_Guid);
		imgui::InputText("m_Name", flipbook.m_Name);

		ImGui::Separator();

		imgui::InputText("m_Shader", flipbook.m_Shader);
		imgui::InputText("m_Texture2D", flipbook.m_Texture2D);

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
					flipbook.m_Texture2D = str::Guid::Create("f9dcfdd6014ac52812e21e73f232b7f9");

					auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
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
			auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
			const str::Name& name = windowComponent.m_Asset.m_Name;

			eng::SelectFileSettings settings;
			settings.m_Title = "Open Flipbook";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::GetPath(str::EPath::Assets);
			settings.m_Path += name;

			const str::Path filepath = eng::SelectFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				eng::Visitor visitor;
				visitor.LoadFromFile(filepath);
				eng::FlipbookAssetLoader loader;
				loader.Load(windowComponent.m_Asset, visitor);
			}

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
			auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
			const str::Name& name = windowComponent.m_Asset.m_Name;

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Flipbook";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::GetPath(str::EPath::Assets);
			settings.m_Path += name;

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveAsset(windowComponent.m_Asset, filepath);
			}

			world.RemoveComponent<editor::FlipbookAssetSaveComponent>(entity);
		}
	};

	void DrawPreviewer(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;
		if (!flipbook.m_Texture2D.IsValid())
			return;

		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.ReadAsset<eng::Texture2DAsset>(flipbook.m_Texture2D);
		if (!textureAsset)
			return;

		const Vector2f imageSize = ToImageSize(ImGui::GetContentRegionAvail(), *textureAsset);
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
		auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(entity);
		eng::FlipbookAsset& flipbook = windowComponent.m_Asset;
		if (!flipbook.m_Texture2D.IsValid())
			return;

		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		const auto* textureAsset = assetManager.ReadAsset<eng::Texture2DAsset>(flipbook.m_Texture2D);
		if (!textureAsset)
			return;

		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const Vector2f imageSize = ToImageSize(regionSize, *textureAsset);
		const ImVec2 regionMin = ImGui::GetCursorScreenPos();
		const ImVec2 regionMax = ImVec2(regionMin.x + imageSize.x, regionMin.y + imageSize.y);

		imgui::Image(textureAsset->m_TextureId, imageSize);

		// draw frames of the flipbook
		for (const eng::FlipbookFrame& frame : flipbook.m_Frames)
		{
			Vector2f min = frame.m_Position;
			Vector2f max = min + frame.m_Size;
			min.x = math::Remap(min.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			min.y = math::Remap(min.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);
			max.x = math::Remap(max.x, 0.f, (float)textureAsset->m_Width, regionMin.x, regionMax.x);
			max.y = math::Remap(max.y, 0.f, (float)textureAsset->m_Height, regionMax.y, regionMin.y);

			imgui::AddRect({ min, max }, Vector4f(1.0f, 1.0f, 0.4f, 1.0f));
		}

		// draw frames to be extracted
		const auto& extractorComponent = world.ReadComponent<editor::FlipbookBatchingComponent>(entity);
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

				imgui::AddRect({ min, max }, Vector4f(1.0f, 0.4f, 0.0f, 1.0f));
			}
		}
	};
}

void editor::FlipbookEditorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::FlipbookWindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Flipbook Editor");
		world.AddComponent<editor::FlipbookBatchingComponent>(windowEntity);

		auto& window = world.AddComponent<editor::FlipbookWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Flipbook Editor##flipbook", identifier);
		window.m_BatchingLabel  = ToLabel("Batching##flipbook", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##flipbook", identifier);
		window.m_PreviewerLabel = ToLabel("Previewer##flipbook", identifier);
		window.m_TextureLabel   = ToLabel("Texture##flipbook", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::FlipbookWindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::FlipbookWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::FlipbookWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::FlipbookWindowComponent>(windowEntity);

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

				ImGuiID textureId, inspectorId, previewerId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &textureId, &inspectorId);
				ImGui::DockBuilderSplitNode(inspectorId, ImGuiDir_Up, 0.6f, &inspectorId, &previewerId);
				ImGui::DockBuilderDockWindow(windowComponent.m_TextureLabel.c_str(), textureId);
				ImGui::DockBuilderDockWindow(windowComponent.m_BatchingLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_PreviewerLabel.c_str(), previewerId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_BatchingLabel.c_str()))
			DrawBatcher(world, windowEntity);
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