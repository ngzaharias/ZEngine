#pragma once

#include "ECS/System.h"

namespace client::settings
{
	struct DebugSingleton;
}

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace eng
{
	struct LinesSingleton;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace hexmap
{
	struct LayerComponent;
	struct RootComponent;
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
			const client::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}