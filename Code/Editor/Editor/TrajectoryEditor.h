#pragma once

#include <Core/Array.h>
#include <Core/Trajectory.h>
#include <Core/Vector.h>

#include <ECS/Component.h>
#include <ECS/System.h>

#include <Engine/TrajectoryAsset.h>

namespace eng
{
	class AssetManager;
}

namespace projectile
{
	struct CreateRequestComponent;
}

namespace editor
{
	struct TrajectoryWindowRequestComponent;

	struct TrajectoryAssetOpenComponent : public ecs::Component<TrajectoryAssetOpenComponent> { };
	struct TrajectoryAssetSaveComponent : public ecs::Component<TrajectoryAssetSaveComponent> { };
	struct TrajectoryWindowComponent : public ecs::Component<TrajectoryWindowComponent> 
	{ 
		eng::TrajectoryAsset m_Asset;

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};
	};

	class TrajectoryEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			editor::TrajectoryAssetOpenComponent,
			editor::TrajectoryAssetSaveComponent,
			editor::TrajectoryWindowComponent,
			projectile::CreateRequestComponent,
			const editor::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};