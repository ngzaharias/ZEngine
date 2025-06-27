#pragma once

#include "Game/Application.h"
#include "Editor/Editor.h"

namespace editor
{
	class Application : public game::Application
	{
	public:
		Application();
		~Application();

	protected:
		void Register() override;

		void Initialise() override;
		void Shutdown() override;

		void Update(const GameTime& gameTime) override;

	protected:
		editor::Editor m_Editor;
	};
}
