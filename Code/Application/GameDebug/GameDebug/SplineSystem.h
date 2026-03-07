#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace debug
{
	struct SplineWindowComponent;
	struct SplineWindowEvent;
}

namespace ecs
{
	struct NameComponent;
}

namespace debug
{
	class SplineSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::SplineWindowComponent,
			ecs::NameComponent>
			::Read<
			debug::SplineWindowEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}