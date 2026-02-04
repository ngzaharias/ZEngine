#include "InputEditorPCH.h"
#include "InputEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "InputEditor/InputEditorWindowComponent.h"
#include "InputEditor/InputEditorWindowRequest.h"
#include "InputEditor/InputEditorWindowSystem.h"

void editor::input::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::input::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::input::WindowRequest>();
	}

	// systems
	{
		world.RegisterSystem<editor::input::WindowSystem>();
	}
}