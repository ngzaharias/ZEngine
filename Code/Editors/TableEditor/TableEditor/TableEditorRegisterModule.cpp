#include "TableEditorPCH.h"
#include "TableEditor/TableEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TableEditor/TableEditorWindowComponent.h"
#include "TableEditor/TableEditorWindowEvent.h"
#include "TableEditor/TableEditorWindowSystem.h"

void editor::table::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::table::WindowComponent>();
	world.RegisterEvent<editor::table::WindowEvent>();
	world.RegisterSystem<editor::table::WindowSystem>();
}