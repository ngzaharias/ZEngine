#pragma once

#include "ECS/System.h"

namespace clt::settings
{
	struct DebugSingleton;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct LinesSingleton;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace hexmap
{
	struct LayerComponent;
	struct RootComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace hexmap
{
	class LoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			ecs::NameComponent,
			eng::level::EntityComponent,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			hexmap::LayerComponent,
			const hexmap::RootComponent,
			// Singletons
			eng::LinesSingleton,
			const clt::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}