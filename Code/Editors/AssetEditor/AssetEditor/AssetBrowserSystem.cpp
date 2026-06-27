#include "AssetPCH.h"
#include "AssetEditor/AssetBrowserSystem.h"

#include "AssetEditor/AssetBrowserOpenEvent.h"
#include "AssetEditor/AssetBrowserComponent.h"
#include "Core/SortHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Icons/Icons.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

#include <filesystem>

namespace
{
	using World = editor::assets::BrowserSystem::World;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	void Icon(const icon::Data& icon, const Vector2f& size = Vector2f(22.f))
	{
		imgui::Image(icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
	}

	void Sort(Array<str::Path>& value)
	{
		std::sort(value.begin(), value.end(), 
			[](const str::Path& a, const str::Path& b) -> int
			{
				return a < b;
				//return sort::AlphaNumeric(a, b) < 0;
			});
	}

	void QSort(Array<str::Path>& value)
	{
		std::qsort(
			value.GetData(),
			value.GetCount(),
			sizeof(str::Path),
			[](const void* a, const void* b) -> int
			{
				const auto& arg1 = *static_cast<const str::Path*>(a);
				const auto& arg2 = *static_cast<const str::Path*>(b);
				return sort::AlphaNumeric(arg1, arg2);
			});
	}

	// must copy the folder
	void LoadFolder(const str::Path folder, editor::assets::BrowserComponent& data)
	{
		Z_PANIC(folder.IsDirectory(), "Trying to open a file instead of a folder!");
		const str::Path& root = str::GetPath(str::EPath::Assets);

		str::Path subpath;
		std::filesystem::path path = folder.ToChar();

		data.m_Current = folder;
		data.m_Files.RemoveAll();
		data.m_Folders.RemoveAll();
		data.m_Parents.RemoveAll();
		
		str::Path parent = folder;
		while (true)
		{
			data.m_Parents.Append(parent);
			if (parent == root)
				break;
			parent = parent.GetParent();
		};

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
		{
			subpath = entry.path().string();
			if (subpath.GetFileExtension() == ".asset")
			{
				data.m_Files.Append(subpath);
			}
			else if (subpath.IsDirectory())
			{
				data.m_Folders.Append(subpath);
			}
		}

		Sort(data.m_Files);
		Sort(data.m_Folders);
	}
}

void editor::assets::BrowserSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags = 0;
	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const auto& request : world.Events<const editor::assets::BrowserOpenEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Asset Browser");

		auto& window = world.AddComponent<editor::assets::BrowserComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Asset Browser", identifier);
		LoadFolder(str::GetPath(str::EPath::Assets), window);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::assets::BrowserComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<editor::assets::BrowserComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::assets::BrowserComponent>>())
	{
		auto& window = view.WriteRequired<editor::assets::BrowserComponent>();

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			// navigation bar
			ImGui::SetWindowFontScale(1.6f);
			for (auto&& [i, parentPath] : enumerate::Reverse(window.m_Parents))
			{
				const str::Path parentName = parentPath.GetStem();
				if (ImGui::Button(parentName.ToChar()))
				{
					LoadFolder(parentPath, window);
					break;
				}

				if (i != 0)
				{
					ImGui::SameLine(0.f, 1.f);
					ImGui::Text("/");
					ImGui::SameLine(0.f, 1.f);
				}
			}

			ImGui::Separator();

			// folders
			for (const str::Path& folderPath : window.m_Folders)
			{
				constexpr Vector2f size = Vector2f(22.f);
				const str::String label = str::String(folderPath.GetStem());
				Icon(icon::FOLDER_EMPTY, size);
				ImGui::SameLine();
				ImGui::Selectable(label.c_str());

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					LoadFolder(folderPath, window);
					break;
				}
			}

			// files
			for (const str::Path& filepath : window.m_Files)
			{
				constexpr Vector2f size = Vector2f(22.f);
				const str::String label = str::String(filepath.GetFileNameNoExtension());
				Icon(icon::FILE_BLANK, size);
				ImGui::SameLine();
				ImGui::Selectable(label.c_str());
				//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				//{
				//	ImGui::SetDragDropPayload("eng::AssetFile", &file, sizeof(eng::AssetFile));
				//	ImGui::EndDragDropSource();
				//}
			}
			ImGui::SetWindowFontScale(1.f);
		}
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(view);
	}
}