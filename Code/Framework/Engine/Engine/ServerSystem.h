#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct ServerComponent;
}

namespace eng
{
	/// \brief System that adds the ServerComponent that is only present on server.
	class ServerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<eng::ServerComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);
	};
};