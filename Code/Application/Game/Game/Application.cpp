#include "GamePCH.h"
#include "Game/Application.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationCloseRequestEvent.h"

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
		m_AssetManager, 
		m_ImguiManager,
		m_NetworkManager,
		m_PhysicsManager,
		m_PlatformManager,
		m_PrototypeManager,
		m_TableHeadmaster,
		m_WindowManager,
		m_ComponentSerializer };

	svr::Dependencies serverDependencies = {
		m_AssetManager,
		m_NetworkManager,
		m_PhysicsManager,
		m_PrototypeManager,
		m_ComponentSerializer };

	gui::Dependencies guiDependencies = {
		m_PrototypeManager,
		m_TableHeadmaster,
		m_GameClient.m_UIManager,
		m_WindowManager };

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

void game::Application::PreUpdate(const GameTime& gameTime)
{
	eng::Application::PreUpdate(gameTime);

	m_GameClient.PreUpdate(gameTime);
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

void game::Application::PostUpdate(const GameTime& gameTime)
{
	eng::Application::PostUpdate(gameTime);

	m_GameClient.PostUpdate(gameTime);
}

bool game::Application::ShouldClose()
{
	if (base::ShouldClose())
		return true;

	using World = ecs::WorldView<eng::application::CloseRequestEvent>;
	World world = m_GameClient.m_EntityWorld.GetWorldView<World>();
	return world.HasAny<eng::application::CloseRequestEvent>();
}