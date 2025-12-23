#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::settings
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
			eng::LinesSingleton,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			hexmap::LayerComponent>
			::Read<
			client::settings::DebugSingleton,
			hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}