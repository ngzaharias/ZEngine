#pragma once

#include "Core/Array.h"
#include "Core/Trajectory.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/TrajectoryAsset.h"
#include "Math/Vector.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

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
		int32 m_Identifier = 0;
		eng::TrajectoryAsset m_Asset = {};

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
			ecs::NameComponent,
			editor::TrajectoryAssetOpenComponent,
			editor::TrajectoryAssetSaveComponent,
			editor::TrajectoryWindowComponent,
			projectile::CreateRequestComponent,
			const editor::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};