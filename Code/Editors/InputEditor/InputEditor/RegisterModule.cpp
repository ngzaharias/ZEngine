#include "InputEditorPCH.h"
#include "InputEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "InputEditor/InputEditorWindowComponent.h"
#include "InputEditor/InputEditorWindowRequest.h"
#include "InputEditor/InputEditorWindowSystem.h"

void editor::input::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::input::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::input::WindowRequest>();
	}

	// singleton
	{
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::input::WindowSystem>();
	}
}