#include "TrajectoryEditorPCH.h"
#include "TrajectoryEditor/TrajectoryEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TrajectoryEditor/TrajectoryEditorAssetOpenComponent.h"
#include "TrajectoryEditor/TrajectoryEditorAssetSaveComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowRequest.h"
#include "TrajectoryEditor/TrajectoryEditorWindowSystem.h"

void editor::trajectory::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::trajectory::AssetOpenComponent>();
		world.RegisterComponent<editor::trajectory::AssetSaveComponent>();
		world.RegisterComponent<editor::trajectory::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::trajectory::WindowRequest>();
	}

	// systems
	{
		world.RegisterSystem<editor::trajectory::WindowSystem>();
	}
}