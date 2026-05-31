#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::settings
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

namespace eng::level
{
	struct EntityComponent;
}

namespace shared::hexmap
{
	struct LayerComponent;
	struct RootComponent;
}

namespace shared::hexmap
{
	class LoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::level::EntityComponent,
			eng::LinesComponent,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			shared::hexmap::LayerComponent>
			::Read<
			client::settings::DebugComponent,
			shared::hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}