#include "TableEditorPCH.h"
#include "TableEditor/TableEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TableEditor/TableEditorWindowComponent.h"
#include "TableEditor/TableEditorWindowRequest.h"
#include "TableEditor/TableEditorWindowSystem.h"

void editor::table::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::table::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::table::WindowRequest>();
	}

	// systems
	{
		world.RegisterSystem<editor::table::WindowSystem>();
	}
}