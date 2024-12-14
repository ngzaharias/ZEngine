#pragma once

#include "ECS/System.h"

namespace clt::settings
{
	struct DebugComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct LinesComponent;
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
			ecs::NameComponent,
			eng::level::EntityComponent,
			eng::LinesComponent,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			hexmap::LayerComponent,
			const clt::settings::DebugComponent,
			const hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}