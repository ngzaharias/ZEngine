#pragma once

#include "Engine/Application.h"
#include "GameClient/GameClient.h"
#include "GameServer/GameServer.h"
#include "GameDebug/GameDebug.h"

namespace game
{
	class Application : public eng::Application
	{
		using base = eng::Application;

	public:
		Application();
		~Application() override;

	protected:
		void Register() override;

		void Initialise() override;
		void Shutdown() override;

		void PreUpdate(const GameTime& gameTime) override;
		void Update(const GameTime& gameTime) override;
		void PostUpdate(const GameTime& gameTime) override;

		bool ShouldClose() override;

	protected:
		clt::GameClient m_GameClient;
		dbg::GameDebug m_GameDebug;
		svr::GameServer m_GameServer;
	};
}
