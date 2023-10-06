#pragma once

#include <Core/Array.h>
#include <Core/Trajectory.h>
#include <Core/Vector.h>

#include <ECS/Component.h>
#include <ECS/System.h>

namespace projectile
{
	struct CreateRequestComponent;
}

namespace editor
{
	struct TrajectoryWindowRequestComponent;

	struct TrajectoryWindowComponent : public ecs::Component<TrajectoryWindowComponent> 
	{ 
		int32 m_WindowId = 0;
		Array<Vector2f> m_Positions = { };

		//eng::TrajectoryAsset m_Asset;

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};
	};

	class TrajectoryEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			editor::TrajectoryWindowComponent,
			projectile::CreateRequestComponent,
			const editor::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		Array<int32> m_UnusedIds = { };
		int32 m_NextId = 0;
	};
};