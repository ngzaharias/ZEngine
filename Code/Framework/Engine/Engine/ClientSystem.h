#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct ClientComponent;
}

namespace eng
{
	/// \brief System that adds the ClientComponent that is only present on client.
	class ClientSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<eng::ClientComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);
	};
};