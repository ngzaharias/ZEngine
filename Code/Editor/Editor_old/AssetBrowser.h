#pragma once

#include <Core/Array.h>
#include <Core/Guid.h>
#include <Core/Path.h>
#include <Core/Set.h>
#include <Core/String.h>

#include <Engine/Hash.h>
#include <Engine/System.h>
#include <Engine/TypeInfo.h>

#include <filesystem>

namespace asset
{
	class Manager;
	struct Entry;
}

namespace editor
{
	class FlipbookEditor;
	class SpriteEditor;
	class SpriteExtractor;

	class AssetBrowser final : public ecs::System
	{
		struct Entry
		{
			bool operator==(const Entry& rhs) const;
			bool operator<(const Entry& rhs) const;

			str::Guid m_Guid = { };
			str::String m_Name = { };
			str::Path m_Path = { };
			TypeId m_TypeId = { };
			bool m_IsDirectory = false;
		};

		using Selection = Array<int32>;

	public:
		AssetBrowser(
			asset::Manager&,
			editor::FlipbookEditor&,
			editor::SpriteEditor&,
			editor::SpriteExtractor&);
		~AssetBrowser();

		void Initialise() override;
		void Destroy() override;

		void Update(const GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Command_ContextMenu();
		void Command_Create();
		void Command_Import();
		void Command_Open();
		void Command_ReloadAll();
		void Command_Select(const int32 index);

		void ContextMenu_Common(const Selection& selection);
		void ContextMenu_Sprite(const Selection& selection);
		void ContextMenu_Texture(const Selection& selection);

		void Render();
		void Render_ContextMenu();
		void Render_Entry(const int32 index);
		void Render_MenuBar();

	private:
		asset::Manager& m_AssetManager;
		editor::FlipbookEditor& m_FlipbookEditor;
		editor::SpriteEditor& m_SpriteEditor;
		editor::SpriteExtractor& m_SpriteExtractor;

		str::Path m_LastImportPath = "Assets";

		str::Path m_PathCurrent = "Assets";
		Array<str::String> m_PathHistory = { "Assets" };

		Set<Entry> m_DirectoryEntries = { };
		Set<core::Hash> m_DirectoryHash = { };

		Selection m_Selection = { };

		bool m_IsVisible = false;
	};
}