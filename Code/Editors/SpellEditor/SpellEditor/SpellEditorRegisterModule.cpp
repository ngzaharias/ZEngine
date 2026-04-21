#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "SpellEditor/SpellEditorWindowComponent.h"
#include "SpellEditor/SpellEditorWindowEvent.h"
#include "SpellEditor/SpellEditorWindowSystem.h"

void editor::spell::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::spell::WindowComponent>();
	world.RegisterEvent<editor::spell::WindowEvent>();
	world.RegisterSystem<editor::spell::WindowSystem>();
}