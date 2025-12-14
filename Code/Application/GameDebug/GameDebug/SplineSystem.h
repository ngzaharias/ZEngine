#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace debug
{
	struct SplineWindowComponent;
	struct SplineWindowRequest;
}

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace debug
{
	class SplineSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::SplineWindowComponent,
			ecs::NameComponent,
			const debug::SplineWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}