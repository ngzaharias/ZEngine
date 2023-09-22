#pragma once

#include <Game/Application.h>

namespace editor
{
	class Application : public game::Application
	{
	public:
		Application();
		~Application();

	private:
		void Register() override;

		void Initialise() override;
		void Shutdown() override;

		void Update(const GameTime& gameTime) override;
	};
}
