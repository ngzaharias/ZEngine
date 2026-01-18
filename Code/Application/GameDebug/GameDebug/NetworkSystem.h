#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace gamestate
{
	struct ChangeRequest;
}

namespace net
{
	class NetworkManager;
}

namespace debug
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequest;

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::NetworkWindowComponent,
			ecs::NameComponent,
			net::NetworkManager>
			::Read<
			debug::NetworkWindowRequest>;

		void Initialise(World& world);
		
		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}