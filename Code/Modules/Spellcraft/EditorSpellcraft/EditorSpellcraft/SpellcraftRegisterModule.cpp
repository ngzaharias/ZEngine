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

void editor::spellcraft::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::spellcraft::GraphComponent>();
	world.RegisterComponent<editor::spellcraft::WindowComponent>();
	world.RegisterEvent<editor::spellcraft::LinkCreateEvent>();
	world.RegisterEvent<editor::spellcraft::LinkDestroyEvent>();
	world.RegisterEvent<editor::spellcraft::NodeCreateEvent>();
	world.RegisterEvent<editor::spellcraft::WindowEvent>();
	world.RegisterSystem<editor::spellcraft::GraphSystem>();
	world.RegisterSystem<editor::spellcraft::WindowSystem>();
}