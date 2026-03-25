#include "ThemeEditorPCH.h"
#include "ThemeEditor/ThemeEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ThemeEditor/ThemeEditorWindowComponent.h"
#include "ThemeEditor/ThemeEditorWindowEvent.h"
#include "ThemeEditor/ThemeEditorWindowSystem.h"

void editor::theme::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::theme::WindowComponent>();
	world.RegisterEvent<editor::theme::WindowEvent>();
	world.RegisterSystem<editor::theme::WindowSystem>();
}