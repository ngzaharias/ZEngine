#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace editor::settings
{
	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Singletons
			editor::settings::LocalSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};