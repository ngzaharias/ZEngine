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
	class Host;
	class Peer;
}

namespace debug
{
	struct NetworkEvent;
	struct NetworkWindowComponent;
	struct NetworkWindowRequest;
}

namespace debug
{
	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::NetworkEvent,
			debug::NetworkWindowComponent,
			ecs::NameComponent,
			net::Host,
			net::Peer>
			::Read<
			debug::NetworkWindowRequest>;

		NetworkSystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Initialise(World& world);
		
		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
		imgui::Identifier m_WindowIds = {};
	};
}