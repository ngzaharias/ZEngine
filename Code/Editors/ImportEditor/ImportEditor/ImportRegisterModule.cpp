#include "ImportPCH.h"
#include "ImportEditor/ImportRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ImportEditor/ImportAdvanceEvent.h"
#include "ImportEditor/ImportQueueComponent.h"
#include "ImportEditor/ImportQueueSystem.h"
#include "ImportEditor/ImportStaticMeshComponent.h"
#include "ImportEditor/ImportStaticMeshSystem.h"

void editor::importer::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::importer::QueueComponent>();
	world.RegisterComponent<editor::importer::StaticMeshComponent>();
	world.RegisterEvent<editor::importer::AdvanceEvent>();
	world.RegisterSystem<editor::importer::QueueSystem>();
	world.RegisterSystem<editor::importer::StaticMeshSystem>();
}