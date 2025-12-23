#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
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
		using World = ecs::WorldView
			::Write<
			debug::NetworkWindowComponent,
			ecs::NameComponent,
			gamestate::ChangeRequest>
			::Read<
			debug::NetworkWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}