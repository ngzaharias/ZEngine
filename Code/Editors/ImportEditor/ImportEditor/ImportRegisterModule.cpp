#include "ImportPCH.h"
#include "ImportEditor/ImportRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ImportEditor/ImportWindowComponent.h"
#include "ImportEditor/ImportWindowSystem.h"

void editor::importer::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::importer::WindowComponent>();
	world.RegisterSystem<editor::importer::WindowSystem>();
}