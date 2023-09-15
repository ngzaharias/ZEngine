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

namespace dbg
{
	struct TrajectoryWindowComponent : public ecs::Component<TrajectoryWindowComponent> 
	{ 
		Array<Vector2f> m_Positions = { };
	};

	class TrajectorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::TrajectoryWindowComponent,
			const dbg::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};