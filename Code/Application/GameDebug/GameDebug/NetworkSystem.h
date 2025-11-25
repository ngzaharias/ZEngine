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

namespace dbg
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequest;

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::ChangeRequest,
			const dbg::NetworkWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}