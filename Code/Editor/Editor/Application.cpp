#include "EditorPCH.h"
#include "Editor/Application.h"

#include <GameClient/GameClient.h>

#include <GameServer/GameServer.h>

editor::Application::Application()
	: m_Editor(m_GameClient.m_EntityWorld, m_GameServer.m_EntityWorld)
{
}

editor::Application::~Application()
{
}

void editor::Application::Register()
{
	PROFILE_FUNCTION();
	game::Application::Register();

	m_Editor.Register();
}

void editor::Application::Initialise()
{
	PROFILE_FUNCTION();
	game::Application::Initialise();

	m_Editor.Initialise();
}

void editor::Application::Shutdown()
{
	PROFILE_FUNCTION();
	m_Editor.Shutdown();

	return game::Application::Shutdown();
}

void editor::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
	game::Application::Update(gameTime);

	m_Editor.Update(gameTime);
}
