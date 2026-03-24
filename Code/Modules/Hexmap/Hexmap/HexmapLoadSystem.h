#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::settings
{
	struct DebugStaticComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct LinesStaticComponent;
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
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::level::EntityComponent,
			eng::LinesStaticComponent,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			hexmap::LayerComponent>
			::Read<
			client::settings::DebugStaticComponent,
			hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}