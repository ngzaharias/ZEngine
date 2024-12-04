#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace gamestate
{
	struct RequestComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace dbg
{
	struct NetworkWindowComponent;
	struct NetworkWindowRequestComponent;

	class NetworkSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::RequestComponent,
			const dbg::NetworkWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}