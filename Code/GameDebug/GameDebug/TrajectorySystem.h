#pragma once

#include <Core/Array.h>
#include <Core/Trajectory.h>
#include <Core/Vector.h>

#include <ECS/Component.h>
#include <ECS/System.h>

namespace dbg
{
	struct TrajectoryWindowRequestComponent;
}

namespace projectile
{
	struct SpawnRequestComponent;
}

namespace dbg
{
	struct TrajectoryWindowComponent : public ecs::Component<TrajectoryWindowComponent> 
	{ 
		int32 m_WindowId = 0;
		Array<Vector2f> m_Positions = { };
	};

	class TrajectorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::TrajectoryWindowComponent,
			projectile::SpawnRequestComponent,
			const dbg::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		Array<int32> m_UnusedIds = { };
		int32 m_NextId = 0;
	};
};