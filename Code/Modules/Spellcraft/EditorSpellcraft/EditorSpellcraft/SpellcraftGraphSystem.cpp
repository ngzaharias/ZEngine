#include "SpellcraftPCH.h"
#include "EditorSpellcraft/SpellcraftGraphSystem.h"

#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "EditorSpellcraft/SpellcraftGraphComponent.h"
#include "EditorSpellcraft/SpellcraftLinkCreateEvent.h"
#include "EditorSpellcraft/SpellcraftLinkDestroyEvent.h"
#include "EditorSpellcraft/SpellcraftNodeCreateEvent.h"

void editor::spellcraft::GraphSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const editor::spellcraft::WindowComponent>>())
	{
		auto& graphComponent = world.AddComponent<editor::spellcraft::GraphComponent>(view);
		graphComponent.m_Graph.Initialise();
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::spellcraft::WindowComponent>>())
		world.RemoveComponent<editor::spellcraft::GraphComponent>(view);

	for (const auto& event : world.Events<const editor::spellcraft::LinkCreateEvent>())
	{
		if (!world.HasComponent<editor::spellcraft::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spellcraft::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		const ngraph::Field& fieldA = graph.GetField(event.m_SourceId);
		const ngraph::Field& fieldB = graph.GetField(event.m_TargetId);
		if (fieldA.m_Type != fieldB.m_Type)
			continue;

		graph.CreateLink(fieldA.m_Type, event.m_SourceId, event.m_TargetId);
	}

	for (const auto& event : world.Events<const editor::spellcraft::LinkDestroyEvent>())
	{
		if (!world.HasComponent<editor::spellcraft::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spellcraft::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		graph.DestroyLink(event.m_LinkId);
	}

	for (const auto& event : world.Events<const editor::spellcraft::NodeCreateEvent>())
	{
		if (!world.HasComponent<editor::spellcraft::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spellcraft::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		graph.CreateNode(event.m_Name);
	}
}