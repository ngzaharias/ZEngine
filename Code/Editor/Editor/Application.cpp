#include "EditorPCH.h"
#include "Editor/Application.h"

editor::Application::Application()
{
}

editor::Application::~Application()
{
}

void editor::Application::Register()
{
	game::Application::Register();

}

void editor::Application::Initialise()
{
	PROFILE_FUNCTION();

	game::Application::Initialise();
}

void editor::Application::Shutdown()
{
	PROFILE_FUNCTION();

	return game::Application::Shutdown();
}

void editor::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	game::Application::Update(gameTime);
}
