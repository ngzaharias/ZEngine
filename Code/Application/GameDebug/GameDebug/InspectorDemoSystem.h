#pragma once

#include "ECS/System.h"

namespace debug
{
	struct InspectorDemoRequest;
	struct InspectorDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
}

namespace debug
{
	class InspectorDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::InspectorDemoWindowComponent,
			const debug::InspectorDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};