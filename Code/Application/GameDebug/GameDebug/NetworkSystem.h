#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace gamestate
{
	struct ChangeRequest;
}

namespace ecs
{
	struct NameComponent;
}

namespace debug
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequest;

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::ChangeRequest,
			const debug::NetworkWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}