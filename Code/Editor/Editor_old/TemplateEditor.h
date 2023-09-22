#pragma once

#include <Engine/System.h>
#include <Engine/TomlTypes.h>

#include "Editor/Historian.h"

namespace asset
{
	class Manager;
}

namespace editor
{
	class TemplateEditor final : public ecs::System
	{
	public:
		TemplateEditor(asset::Manager& assetManager);
		~TemplateEditor();

		void Initialise() override;
		void Update(const GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render();
		void Render_MenuBar();
		void Render_Selection();

	private:
		asset::Manager& m_AssetManager;

		toml::Value m_DocumentOld = { };
		toml::Value m_DocumentNew = { };
		bool m_IsVisible = false;
	};
}