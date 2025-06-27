#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace dbg
{
	struct SplineWindowComponent;
	struct SplineWindowRequestComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace dbg
{
	class SplineSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::SplineWindowComponent,
			ecs::NameComponent,
			const dbg::SplineWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}