#pragma once

#include <ECS/System.h>

namespace spell
{
	struct DefinitionMapComponent;

	class DefinitionSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			spell::DefinitionMapComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}