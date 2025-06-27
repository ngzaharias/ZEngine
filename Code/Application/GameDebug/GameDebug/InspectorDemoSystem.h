#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct InspectorDemoRequestComponent;

	struct InspectorDemoWindowComponent : public ecs::Component<InspectorDemoWindowComponent> { };

	class InspectorDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::InspectorDemoWindowComponent,
			const dbg::InspectorDemoRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};