#include "InputEditorPCH.h"
#include "InputEditor/InputEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "InputEditor/InputEditorWindowComponent.h"
#include "InputEditor/InputEditorWindowEvent.h"
#include "InputEditor/InputEditorWindowSystem.h"

void editor::input::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::input::WindowComponent>();
	world.RegisterEvent<editor::input::WindowEvent>();
	world.RegisterSystem<editor::input::WindowSystem>();
}