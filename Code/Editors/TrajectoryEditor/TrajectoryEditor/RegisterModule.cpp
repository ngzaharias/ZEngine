#include "TrajectoryEditorPCH.h"
#include "TrajectoryEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TrajectoryEditor/TrajectoryEditorAssetOpenComponent.h"
#include "TrajectoryEditor/TrajectoryEditorAssetSaveComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowRequest.h"
#include "TrajectoryEditor/TrajectoryEditorWindowSystem.h"

void editor::trajectory::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::trajectory::AssetOpenComponent>();
		entityWorld.RegisterComponent<editor::trajectory::AssetSaveComponent>();
		entityWorld.RegisterComponent<editor::trajectory::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::trajectory::WindowRequest>();
	}

	// singleton
	{
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::trajectory::WindowSystem>();
	}
}