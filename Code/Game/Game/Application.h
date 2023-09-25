#pragma once

#include <Engine/Application.h>

#include <GameClient/GameClient.h>
#include <GameServer/GameServer.h>
#include <GameDebug/GameDebug.h>

namespace game
{
	class Application : public eng::Application
	{
	public:
		Application();
		~Application() override;

	protected:
		void Register() override;

		void Initialise() override;
		void Shutdown() override;

		void Update(const GameTime& gameTime) override;

	protected:
		clt::GameClient m_GameClient;
		dbg::GameDebug m_GameDebug;
		svr::GameServer m_GameServer;
	};
}
