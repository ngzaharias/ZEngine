#include "ThemeEditorPCH.h"
#include "ThemeEditor/ThemeEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ThemeEditor/ThemeEditorWindowComponent.h"
#include "ThemeEditor/ThemeEditorWindowRequest.h"
#include "ThemeEditor/ThemeEditorWindowSystem.h"

void editor::theme::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::theme::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::theme::WindowRequest>();
	}

	// systems
	{
		world.RegisterSystem<editor::theme::WindowSystem>();
	}
}