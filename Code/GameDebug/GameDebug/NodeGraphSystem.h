#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct NodeGraphComponent;
	struct NodeGraphRequestComponent;

	class NodeGraphSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::NodeGraphComponent,
			const dbg::NodeGraphRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}