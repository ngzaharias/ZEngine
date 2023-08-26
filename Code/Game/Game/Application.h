#pragma once

#include <Engine/Application.h>

#include <GameClient/GameClient.h>
#include <GameServer/GameServer.h>
#include <GameDebug/GameDebug.h>

namespace game
{
	class Application final : public eng::Application
	{
	public:
		Application();
		~Application() override;

	protected:
		void Register() override;
		void Initialise() override;
		void Shutdown() override;
		void Update(const GameTime& gameTime) override;

	private:
		clt::GameClient m_GameClient;
		dbg::GameDebug m_GameDebug;
		svr::GameServer m_GameServer;
	};
}
