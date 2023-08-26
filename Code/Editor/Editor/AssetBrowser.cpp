#include "EditorPCH.h"
#include "Editor/AssetBrowser.h"

#include "Editor/FlipbookEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Core/Algorithms.h>
#include <Core/SortHelpers.h>
#include <Core/StringHelpers.h>

#include <Engine/AssetManager.h>
#include <Engine/FileHelpers.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/InputComponent.h>
#include <Engine/PhysicsMaterialAsset.h>
#include <Engine/SoundAsset.h>
#include <Engine/SpriteAsset.h>
#include <Engine/Texture2DAsset.h>
#include <Engine/TomlHelpers.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// #todo: preview icons
// #todo: drag/drop -> inspector/scene
// #todo: ensure unique filename
// #todo: rename
// #todo: bold font for directory path

namespace
{
	constexpr float s_IconSize = 10.f;
	constexpr float s_RowHeight = 20.f;

	constexpr wchar_t* s_Extension = L".asset";

	const str::String strAsset = "*.asset";
	const str::String strSounds = "*.ogg";
	const str::String strTextures = "*.png";
	const str::String strAll = strSounds + ";" + strTextures;

	str::String stringBuffer = { };

	enum class EResult
	{
		None,
		LeftClick,
		LeftClick_x2,
		RightClick,
	};

	static const Array<std::string> s_AssetFilters =
	{
		"All Files (" + strAll + ")", strAll,
		"Sounds (" + strSounds + ")", strSounds,
		"Textures (" + strTextures + ")", strTextures,
	};

	template<typename Type>
	bool Contains(const Array<Type>& container, const Type& value)
	{
		return std::find(container.begin(), container.end(), value) != container.end();
	}

	//bool MenuBarItem(const char* text, const sf::Texture& texture)
	//{
	//	const ImVec2 itemSize = { 100.f, 40.f };

	//	ImGui::PushID(text);
	//	const ImVec2 cursorBefore = ImGui::GetCursorPos();
	//	const bool result = ImGui::Selectable("", false, 0, itemSize);
	//	const ImVec2 cursorAfter = ImGui::GetCursorPos();

	//	// image
	//	{
	//		ImGui::SetCursorPosX(cursorBefore.x);
	//		ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (texture.getSize().y * 0.5f));
	//		ImGui::Image(texture);
	//	}

	//	// text
	//	{
	//		const float fontHeight = ImGui::GetFontSize();
	//		ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (fontHeight * 0.5f));
	//		ImGui::Text(text);
	//	}

	//	ImGui::SetCursorPos(cursorAfter);
	//	ImGui::PopID();
	//	return result;
	//}

	void RebuildPathAtIndex(str::Path& out_Path, Array<str::String>& out_History, const int32 index)
	{
		if (index < 0 || index >= out_History.GetCount())
			return;

		out_Path.clear();
		for (auto&& [i, string] : enumerate::Forward(out_History))
		{
			out_Path += string + '/';
			if (i == index)
				break;
		}

		out_History.Resize(index + 1);
	}
}

bool editor::AssetBrowser::Entry::operator==(const Entry& rhs) const
{
	return m_Name == rhs.m_Name;
}

bool editor::AssetBrowser::Entry::operator<(const Entry& rhs) const
{
	if (m_IsDirectory != rhs.m_IsDirectory)
		return m_IsDirectory;

	return sort::AlphaNumeric(m_Name, rhs.m_Name) < 0;
}

editor::AssetBrowser::AssetBrowser(
	asset::Manager& assetManager,
	editor::FlipbookEditor& flipbookEditor,
	editor::SpriteEditor& spriteEditor,
	editor::SpriteExtractor& spriteExtractor)
	: m_AssetManager(assetManager)
	, m_FlipbookEditor(flipbookEditor)
	, m_SpriteEditor(spriteEditor)
	, m_SpriteExtractor(spriteExtractor)
{
}

editor::AssetBrowser::~AssetBrowser()
{
}

void editor::AssetBrowser::Initialise()
{
	//iconBack = new sf::Texture();
	//iconFile = new sf::Texture();
	//iconFolder = new sf::Texture();

	//iconBack->loadFromFile("Code\\Editor\\Icons\\arrow_left.png");
	//iconFile->loadFromFile("Code\\Editor\\Icons\\file.png");
	//iconFolder->loadFromFile("Code\\Editor\\Icons\\folder.png");
}

void editor::AssetBrowser::Destroy()
{
}

void editor::AssetBrowser::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_PathCurrent.empty())
	{
		Set<core::Hash> directoryHash;
		for (const auto& entry : std::filesystem::directory_iterator(m_PathCurrent))
		{
			const str::Path& path = entry.path();
			if (!entry.is_directory() && path.extension() != s_Extension)
				break;

			const str::String name = path.stem().u8string();
			const core::Hash hash = str::ToHash(name);
			directoryHash.emplace(hash);
		}

		Set<core::Hash> difference;
		core::Difference(directoryHash, m_DirectoryHash, difference);
		core::Difference(m_DirectoryHash, directoryHash, difference);

		if (!difference.empty())
		{
			m_DirectoryHash = directoryHash;
			Command_ReloadAll();
		}
	}

	// #todo: replace sf::Texture with glfw one
	//Render();
}

void editor::AssetBrowser::Command_ContextMenu()
{
	ImGui::OpenPopup("ContextMenu");
}

void editor::AssetBrowser::Command_Create()
{
	ImGui::OpenPopup("Create");

	if (ImGui::BeginPopup("Create"))
	{
		if (ImGui::BeginMenu("Physics\t"))
		{
			if (ImGui::MenuItem("Material"))
				m_AssetManager.CreateAsset<asset::PhysicsMaterialAsset>("Assets\\Example.asset");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Render\t"))
		{
			if (ImGui::MenuItem("Flipbook"))
				m_AssetManager.CreateAsset<asset::FlipbookAsset>("Assets\\Example.asset");
			if (ImGui::MenuItem("Sprite"))
				m_AssetManager.CreateAsset<asset::SpriteAsset>("Assets\\Example.asset");
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}

void editor::AssetBrowser::Command_Import()
{
	// #todo: support multi-import
	core::SelectFileSettings selectSettings;
	selectSettings.m_Title = "Import Asset";
	selectSettings.m_Directory = m_LastImportPath.string();
	selectSettings.m_Filters = s_AssetFilters;
	selectSettings.m_IsMultiSelect = false;

	Array<str::Path> inputPaths = core::SelectFileDialog(selectSettings);
	if (inputPaths.IsEmpty())
		return;

	core::SaveFileSettings saveSettings;
	saveSettings.m_Title = "Import Location";
	saveSettings.m_Directory = m_PathCurrent.string();
	saveSettings.m_Filters = { "Assets (*.asset)", "*.asset" };

	str::Path outputPath = core::SaveFileDialog(saveSettings);
	if (outputPath.empty())
		return;

	if (!outputPath.has_extension())
		outputPath += s_Extension;

	for (const str::Path& inputPath : inputPaths)
	{
		// #todo: register extensions with file types
		const str::String extension = inputPath.extension().string();
		if (str::ContainsAny_NoCase(extension, { ".png" }))
		{
			m_AssetManager.ImportAsset<asset::Texture2DAsset>(inputPath, outputPath);
		}
		else if (str::ContainsAny_NoCase(extension, { ".ogg" }))
		{
			m_AssetManager.ImportAsset<asset::SoundAsset>(inputPath, outputPath);
		}

		m_AssetManager.LoadFilepath(inputPath, false);
	}

	// #todo: save to config file
	m_LastImportPath = inputPaths.GetLast();
	m_LastImportPath.remove_filename();
}

void editor::AssetBrowser::Command_Open()
{
	for (const auto& index : m_Selection)
	{
		auto entry = m_DirectoryEntries.begin();
		std::advance(entry, index);

		if (entry->m_IsDirectory)
		{
			m_PathCurrent = entry->m_Path;
			m_PathHistory.Append(m_PathCurrent.stem().string());
		}
		else
		{
			switch (entry->m_TypeId)
			{
			case ToTypeId<asset::FlipbookAsset>():
				m_FlipbookEditor.OpenEditor(entry->m_Guid);
				break;
			case ToTypeId<asset::SpriteAsset>():
				m_SpriteEditor.OpenEditor(entry->m_Guid);
				break;
			}
		}
	}
	m_Selection.RemoveAll();
}

void editor::AssetBrowser::Command_ReloadAll()
{
	m_DirectoryEntries.clear();
	for (const auto& entry : std::filesystem::directory_iterator(m_PathCurrent))
	{
		const str::Path& path = entry.path();
		const str::String name = path.stem().u8string();
		if (!entry.is_directory() && path.extension() != s_Extension)
			break;

		str::Guid guid = { };
		TypeId typeId = { };
		if (!entry.is_directory())
		{
			toml::Value document;
			toml::LoadDocument(entry.path(), document);
			guid = toml::Parse<str::Guid>(document, "Guid", {});
			typeId = toml::Parse<int32>(document, "Type", {});
		}

		Entry directoryEntry = {
			guid,
			name,
			path,
			typeId,
			entry.is_directory() };

		m_DirectoryEntries.emplace(std::move(directoryEntry));
	}
}

void editor::AssetBrowser::Command_Select(const int32 index)
{
	const auto view = m_World->m_Registry.view<const input::InputComponent>();
	for (auto& entity : view)
	{
		const auto& inputComponent = view.get<const input::InputComponent>(entity);
		if (inputComponent.IsKeyPressed(input::EKeyboard::Shift_L) && !m_Selection.IsEmpty())
		{
			const int32 first = std::min(index, m_Selection.GetLast());
			const int32 last = std::max(index, m_Selection.GetLast());

			m_Selection.RemoveLast();
			for (int32 i = first; i <= last; ++i)
				m_Selection.Append(i);
		}
		else if (inputComponent.IsKeyPressed(input::EKeyboard::Control_L))
		{
			m_Selection.Append(index);
		}
		else
		{
			m_Selection.RemoveAll();
			m_Selection.Append(index);
		}
	}
}

void editor::AssetBrowser::ContextMenu_Common(const Selection& selection)
{
	ImGui::TextDisabled("Common Actions");
}

void editor::AssetBrowser::ContextMenu_Sprite(const Selection& selection)
{
	ImGui::TextDisabled("Sprite Actions");
	if (ImGui::MenuItem("Create Flipbook..."))
	{
	}
}

void editor::AssetBrowser::ContextMenu_Texture(const Selection& selection)
{
	ImGui::TextDisabled("Texture Actions");
	if (ImGui::MenuItem("Extract Sprites..."))
	{
		auto entry = m_DirectoryEntries.begin();
		std::advance(entry, m_Selection[0]);
		m_SpriteExtractor.OpenDialog(entry->m_Guid);
	}
}

void editor::AssetBrowser::Render()
{
	if (!m_IsVisible)
		return;

	if (ImGui::Begin("Asset Browser", &m_IsVisible))
	{
		Render_MenuBar();

		//if (ImGui::ImageButton(*iconBack, { 16.f, 20.f }))
		//	RebuildPathAtIndex(m_PathCurrent, m_PathHistory, m_PathHistory.GetCount() - 2);

		ImGui::SameLine();

		ImGui::SetWindowFontScale(1.5f);
		{
			const str::String directoryPath = m_PathCurrent.string();
			for (auto&& [i, string] : enumerate::Forward(m_PathHistory))
			{
				stringBuffer = string + '/';
				if (ImGui::Button(stringBuffer.c_str()))
				{
					RebuildPathAtIndex(m_PathCurrent, m_PathHistory, i);
					break;
				}

				if (i != m_PathHistory.GetCount() - 1)
					ImGui::SameLine();
			}
		}
		ImGui::SetWindowFontScale(1.f);

		ImGui::Separator();
		ImGui::Separator();

		constexpr int32 s_TableColumns = 1;
		if (ImGui::BeginChild("Entries"))
		{
			ImGui::BeginTable("Table", s_TableColumns, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
			ImGui::TableNextRow();

			for (int32 i = 0; i < s_TableColumns; ++i)
			{
				ImGui::TableSetColumnIndex(i);
				ImGui::PushItemWidth(-FLT_MIN);
			}

			ImGui::TableSetColumnIndex(0);

			if (s_TableColumns == 1)
			{
				ImGui::Text("Name");
				ImGui::Separator();
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
			}

			for (int32 index = 0; index < m_DirectoryEntries.size(); ++index)
			{
				Render_Entry(index);
				ImGui::TableNextColumn();
			}

			Render_ContextMenu();

			ImGui::EndTable();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void editor::AssetBrowser::Render_ContextMenu()
{
	if (m_Selection.IsEmpty())
		return;

	if (ImGui::BeginPopup("ContextMenu"))
	{
		bool isDirectory = false;
		Nullable<TypeId> typeId = { };
		for (const auto& index : m_Selection)
		{
			auto entry = m_DirectoryEntries.begin();
			std::advance(entry, index);

			if (typeId && entry->m_TypeId != *typeId)
			{
				typeId = { };
				break;
			}
			typeId = entry->m_TypeId;
		}

		if (isDirectory)
		{
			ImGui::TextDisabled("Directory Actions");
		}
		else
		{
			switch (typeId.value_or(0))
			{
			case ToTypeId<asset::FlipbookAsset>():
				ImGui::TextDisabled("Flipbook Actions");
				break;
			case ToTypeId<asset::PhysicsMaterialAsset>():
				ImGui::TextDisabled("Physics Material Actions");
				break;
			case ToTypeId<asset::SoundAsset>():
				ImGui::TextDisabled("Sound Actions");
				break;
			case ToTypeId<asset::SpriteAsset>():
				ContextMenu_Sprite(m_Selection);
				break;
			case ToTypeId<asset::Texture2DAsset>():
				ContextMenu_Texture(m_Selection);
				break;
			}

			ContextMenu_Common(m_Selection);
		}
		ImGui::EndPopup();
	}
}

void editor::AssetBrowser::Render_Entry(const int32 index)
{
	//auto entry = m_DirectoryEntries.begin();
	//std::advance(entry, index);

	//const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	//const ImVec2 itemSize = { ImGui::GetColumnWidth(), s_RowHeight };
	//const float framePadding = 4.f;
	//const sf::Texture& texture = entry->m_IsDirectory ? *iconFolder : *iconFile;

	//EResult result = EResult::None;
	//if (ImGui::BeginChild(entry->m_Name.c_str(), itemSize, false, flags))
	//{
	//	const bool isSelected = std::find_if(m_Selection.begin(), m_Selection.end(),
	//		[&](const auto& selected) -> bool { return selected == index; }) != m_Selection.end();

	//	const ImVec2 cursorBefore = ImGui::GetCursorPos();
	//	ImGui::Selectable("##something", isSelected, 0, itemSize);
	//	const ImVec2 cursorAfter = ImGui::GetCursorPos();

	//	if (ImGui::IsItemHovered())
	//	{
	//		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	//			result = EResult::LeftClick;
	//		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	//			result = EResult::LeftClick_x2;
	//		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	//			result = EResult::RightClick;
	//	}

	//	// image
	//	{
	//		float positionX = cursorBefore.x + framePadding;
	//		float positionY = cursorBefore.y + (itemSize.y * 0.5f) - (texture.getSize().y * 0.5f);
	//		positionY += s_IconSize * 0.7f;
	//		ImGui::SetCursorPosX(positionX);
	//		ImGui::SetCursorPosY(positionY);
	//		ImGui::Image(texture, { s_IconSize, s_IconSize });
	//	}

	//	// text
	//	{
	//		const float fontHeight = ImGui::GetFontSize();
	//		ImGui::SameLine();
	//		ImGui::SetCursorPosY(cursorBefore.y + (itemSize.y * 0.5f) - (fontHeight * 0.5f));
	//		ImGui::Text(entry->m_Name.c_str());
	//	}

	//	ImGui::SetCursorPos(cursorAfter);
	//}
	//ImGui::EndChild();

	//switch (result)
	//{
	//case EResult::LeftClick:
	//	Command_Select(index);
	//	break;
	//case EResult::LeftClick_x2:
	//	if (!Contains(m_Selection, index))
	//		Command_Select(index);
	//	Command_Open();
	//	break;
	//case EResult::RightClick:
	//	if (!Contains(m_Selection, index))
	//		Command_Select(index);
	//	Command_ContextMenu();
	//	break;
	//}
}

void editor::AssetBrowser::Render_MenuBar()
{
	if (ImGui::BeginChild("Menu Bar", { ImGui::GetWindowWidth(), 40.f } ))
	{
		ImGui::GetCurrentWindow()->DC.LayoutType = ImGuiLayoutType_Horizontal;

		//if (MenuBarItem("Create", *iconFile))
		//	Command_Create();

		//if (MenuBarItem("Import...", *iconFolder))
		//	Command_Import();
	}
	ImGui::EndChild();
}