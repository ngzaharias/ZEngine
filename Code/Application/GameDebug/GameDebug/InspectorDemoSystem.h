#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
}

namespace debug
{
	struct InspectorDemoRequest;

	struct InspectorDemoWindowComponent final : public ecs::Component<InspectorDemoWindowComponent> { };

	class InspectorDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::InspectorDemoWindowComponent,
			const debug::InspectorDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};