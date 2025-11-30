#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct InspectorDemoRequest;

	struct InspectorDemoWindowComponent final : public ecs::Component<InspectorDemoWindowComponent> { };

	class InspectorDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::InspectorDemoWindowComponent,
			const dbg::InspectorDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};