#include "GamePCH.h"
#include "Game/Application.h"

game::Application::Application()
	: eng::Application()
	, m_GameClient()
	, m_GameServer()
	, m_GameUI(m_GameClient.m_EntityWorld)
	, m_GameDebug(m_GameClient.m_EntityWorld, m_GameServer.m_EntityWorld)
{
}

game::Application::~Application()
{
}

void game::Application::Register()
{
	PROFILE_FUNCTION();
	eng::Application::Register();

	clt::Dependencies clientDependencies = {
		*m_Window,
		m_AssetManager, 
		m_NetworkManager,
		m_PhysicsManager,
		m_PlatformManager,
		m_PrototypeManager,
		m_TableHeadmaster,
		m_ComponentSerializer };

	svr::Dependencies serverDependencies = {
		m_AssetManager,
		m_NetworkManager,
		m_PhysicsManager,
		m_PrototypeManager,
		m_ComponentSerializer};

	gui::Dependencies guiDependencies = {};

	m_GameClient.Register(clientDependencies);
	m_GameServer.Register(serverDependencies);
	m_GameUI.Register(guiDependencies);
	m_GameDebug.Register();
}

void game::Application::Initialise()
{
	PROFILE_FUNCTION();
	eng::Application::Initialise();

	m_GameServer.m_ReplicationHost.Initialise();
	m_GameClient.m_ReplicationPeer.Initialise();

	m_GameClient.Initialise();
	m_GameServer.Initialise();
	m_GameUI.Initialise();
	m_GameDebug.Initialise();
}

void game::Application::Shutdown()
{
	PROFILE_FUNCTION();

	m_GameDebug.Shutdown();
	m_GameUI.Shutdown();
	m_GameServer.Shutdown();
	m_GameClient.Shutdown();

	m_GameClient.m_ReplicationPeer.Shutdown();
	m_GameServer.m_ReplicationHost.Shutdown();

	eng::Application::Shutdown();
}

void game::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
	eng::Application::Update(gameTime);

	// host first, then peer
	m_GameServer.m_ReplicationHost.Update(gameTime);
	m_GameClient.m_ReplicationPeer.Update(gameTime);

	m_GameClient.Update(gameTime);
	m_GameServer.Update(gameTime);
	m_GameUI.Update(gameTime);
	m_GameDebug.Update(gameTime);
}