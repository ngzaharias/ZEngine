#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorGraphSystem.h"

#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SpellEditor/SpellEditorGraphComponent.h"
#include "SpellEditor/SpellEditorLinkCreateEvent.h"
#include "SpellEditor/SpellEditorLinkDestroyEvent.h"
#include "SpellEditor/SpellEditorNodeCreateEvent.h"

void editor::spell::GraphSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const editor::spell::WindowComponent>>())
	{
		auto& graphComponent = world.AddComponent<editor::spell::GraphComponent>(view);
		graphComponent.m_Graph.Initialise();
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::spell::WindowComponent>>())
		world.RemoveComponent<editor::spell::GraphComponent>(view);

	for (const auto& event : world.Events<editor::spell::LinkCreateEvent>())
	{
		if (!world.HasComponent<editor::spell::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spell::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		const ngraph::Field& fieldA = graph.GetField(event.m_SourceId);
		const ngraph::Field& fieldB = graph.GetField(event.m_TargetId);
		if (fieldA.m_Type != fieldB.m_Type)
			continue;

		graph.CreateLink(event.m_SourceId, event.m_TargetId);
	}

	for (const auto& event : world.Events<editor::spell::LinkDestroyEvent>())
	{
		if (!world.HasComponent<editor::spell::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spell::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		graph.DestroyLink(event.m_LinkId);
	}

	for (const auto& event : world.Events<editor::spell::NodeCreateEvent>())
	{
		if (!world.HasComponent<editor::spell::GraphComponent>(event.m_Entity))
			continue;

		auto& graphComponent = world.WriteComponent<editor::spell::GraphComponent>(event.m_Entity);
		auto& graph = graphComponent.m_Graph;
		graph.CreateNode(event.m_Name);
	}
}