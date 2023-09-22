#pragma once

#include <Engine/System.h>

namespace audio
{
	class SoundSystem;
}

namespace editor
{
	class AssetBrowser;
	class EntityBrowser;
	class GameWindow;
	class Historian;
	class Inspector;
	class NodeGraphEditor;
	class TemplateEditor;

	class MainMenuBar final : public ecs::System
	{
	public:
		MainMenuBar(
			audio::SoundSystem& soundSystem,
			editor::AssetBrowser& assetBrowser,
			editor::EntityBrowser& entityBrowser,
			editor::Historian& historian,
			editor::Inspector& inspector,
			editor::NodeGraphEditor& nodeGraphEditor,
			editor::TemplateEditor& templateEditor);

		void Update(const GameTime& gameTime) override;
		void Render();

	private:
		audio::SoundSystem& m_SoundSystem;
		editor::AssetBrowser& m_AssetBrowser;
		editor::EntityBrowser& m_EntityBrowser;
		editor::Historian& m_Historian;
		editor::Inspector& m_Inspector;
		editor::NodeGraphEditor& m_NodeGraphEditor;
		editor::TemplateEditor& m_TemplateEditor;
	};
}