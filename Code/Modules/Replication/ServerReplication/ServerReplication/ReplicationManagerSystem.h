#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct ReplicationComponent;
}

namespace shared::cursor
{
	struct TransformComponent;
}

namespace shared::voxel
{
	struct ChunkComponent;
}

namespace server::replication
{
	/// \brief System responsible for adding/removing ReplicationComponent.
	/// It is expected that this module will include all other modules and that this system runs towards the end of a frame.
	class ManagerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::ReplicationComponent>
			::Read<
			shared::cursor::TransformComponent,
			shared::voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
