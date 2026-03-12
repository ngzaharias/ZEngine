#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
	struct NameComponent;
}

namespace debug::entity
{
	struct WindowComponent;
	struct WindowEvent;
}

namespace debug::entity
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::entity::WindowComponent,
			ecs::NameComponent>
			::Read<
			debug::entity::WindowEvent>;

		WindowSystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
		imgui::Identifier m_WindowIds = {};
	};
};