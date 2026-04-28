#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "SpellEditor/SpellEditorGraphComponent.h"
#include "SpellEditor/SpellEditorGraphSystem.h"
#include "SpellEditor/SpellEditorLinkCreateEvent.h"
#include "SpellEditor/SpellEditorLinkDestroyEvent.h"
#include "SpellEditor/SpellEditorNodeCreateEvent.h"
#include "SpellEditor/SpellEditorWindowComponent.h"
#include "SpellEditor/SpellEditorWindowEvent.h"
#include "SpellEditor/SpellEditorWindowSystem.h"

void editor::spell::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::spell::GraphComponent>();
	world.RegisterComponent<editor::spell::WindowComponent>();
	world.RegisterEvent<editor::spell::LinkCreateEvent>();
	world.RegisterEvent<editor::spell::LinkDestroyEvent>();
	world.RegisterEvent<editor::spell::NodeCreateEvent>();
	world.RegisterEvent<editor::spell::WindowEvent>();
	world.RegisterSystem<editor::spell::GraphSystem>();
	world.RegisterSystem<editor::spell::WindowSystem>();
}