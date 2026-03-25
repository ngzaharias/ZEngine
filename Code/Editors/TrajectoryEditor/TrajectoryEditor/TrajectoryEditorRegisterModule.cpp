#include "TrajectoryEditorPCH.h"
#include "TrajectoryEditor/TrajectoryEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TrajectoryEditor/TrajectoryEditorAssetOpenComponent.h"
#include "TrajectoryEditor/TrajectoryEditorAssetSaveComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowComponent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowEvent.h"
#include "TrajectoryEditor/TrajectoryEditorWindowSystem.h"

void editor::trajectory::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::trajectory::AssetOpenComponent>();
	world.RegisterComponent<editor::trajectory::AssetSaveComponent>();
	world.RegisterComponent<editor::trajectory::WindowComponent>();
	world.RegisterEvent<editor::trajectory::WindowEvent>();
	world.RegisterSystem<editor::trajectory::WindowSystem>();
}