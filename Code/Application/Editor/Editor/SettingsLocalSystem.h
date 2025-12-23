#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::settings
{
	struct LocalSingleton;
}

namespace editor::settings
{
	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::settings::LocalSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};