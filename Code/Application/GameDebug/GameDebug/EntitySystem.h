#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
	struct NameComponent;
}

namespace debug
{
	struct EntityWindowComponent;
	struct EntityWindowRequest;
}

namespace debug
{
	class EntitySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::EntityWindowComponent,
			ecs::NameComponent>
			::Read<
			debug::EntityWindowRequest>;

		EntitySystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
		imgui::Identifier m_WindowIds = {};
	};
};