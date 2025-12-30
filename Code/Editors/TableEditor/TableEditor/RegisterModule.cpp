#include "TableEditorPCH.h"
#include "TableEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TableEditor/TableEditorWindowComponent.h"
#include "TableEditor/TableEditorWindowRequest.h"
#include "TableEditor/TableEditorWindowSystem.h"

void editor::table::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::table::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::table::WindowRequest>();
	}

	// singleton
	{
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::table::WindowSystem>();
	}
}