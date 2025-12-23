#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct InspectorDemoRequest;
	struct InspectorDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	class InspectorDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::InspectorDemoWindowComponent>
			::Read<
			debug::InspectorDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};