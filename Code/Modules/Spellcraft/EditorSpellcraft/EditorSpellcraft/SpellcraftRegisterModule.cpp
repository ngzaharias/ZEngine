#include "SpellcraftPCH.h"
#include "EditorSpellcraft/SpellcraftRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "EditorSpellcraft/SpellcraftGraphComponent.h"
#include "EditorSpellcraft/SpellcraftGraphSystem.h"
#include "EditorSpellcraft/SpellcraftLinkCreateEvent.h"
#include "EditorSpellcraft/SpellcraftLinkDestroyEvent.h"
#include "EditorSpellcraft/SpellcraftNodeCreateEvent.h"
#include "EditorSpellcraft/SpellcraftWindowComponent.h"
#include "EditorSpellcraft/SpellcraftWindowEvent.h"
#include "EditorSpellcraft/SpellcraftWindowSystem.h"

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