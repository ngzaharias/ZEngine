#pragma once

#include <ECS/System.h>

namespace spell
{
	struct DefinitionMapComponent;
	struct RootComponent;
	struct SubComponent;

	class ExecuteSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			spell::RootComponent,
			spell::SubComponent,
			const spell::DefinitionMapComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}