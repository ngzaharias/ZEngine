#include "InspectorPCH.h"
#include "Inspector/InspectorHistorySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"
#include "Engine/TemplateMapComponent.h"
#include "Inspector/InspectorHistoryComponent.h"
#include "Inspector/InspectorHistoryCreateEvent.h"
#include "Inspector/InspectorHistoryDestroyEvent.h"
#include "Inspector/InspectorHistoryRedoEvent.h"
#include "Inspector/InspectorHistoryUndoEvent.h"
#include "Inspector/InspectorHistoryUpdateEvent.h"

namespace 
{
	void Undo(ecs::EntityWorld& world, editor::inspector::HistoryCreate& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);
		world.DestroyEntity(entity);
	}

	void Redo(ecs::EntityWorld& world, editor::inspector::HistoryCreate& command)
	{
		const ecs::Entity entity = world.CreateEntity();
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.LoadEntity(world, entity, command.m_Data);
	}

	void Undo(ecs::EntityWorld& world, editor::inspector::HistoryDestroy& command)
	{
		const ecs::Entity entity = world.CreateEntity();
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.LoadEntity(world, entity, command.m_Data);
	}

	void Redo(ecs::EntityWorld& world, editor::inspector::HistoryDestroy& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);
		world.DestroyEntity(entity);
	}

	void Undo(ecs::EntityWorld& world, editor::inspector::HistoryUpdate& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);

		str::String redoData;
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(world, entity, redoData);
		manager.SyncEntity(world, entity, command.m_Data);
		command.m_Data = std::move(redoData);
	}

	void Redo(ecs::EntityWorld& world, editor::inspector::HistoryUpdate& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);

		str::String undoData;
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(world, entity, undoData);
		manager.SyncEntity(world, entity, command.m_Data);
		command.m_Data = std::move(undoData);
	}
}

editor::inspector::HistorySystem::HistorySystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::inspector::HistorySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& request : world.Events<editor::inspector::HistoryCreateEvent>())
	{
		HistoryCreate command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(m_World, request.m_Entity, command.m_Data);

		auto& historyComponent = world.WriteComponent<editor::inspector::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (const auto& request : world.Events<editor::inspector::HistoryDestroyEvent>())
	{
		HistoryDestroy command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(m_World, request.m_Entity, command.m_Data);

		auto& historyComponent = world.WriteComponent<editor::inspector::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (auto& request : world.Events<editor::inspector::HistoryUpdateEvent>())
	{
		HistoryUpdate command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		command.m_Data = request.m_Data;

		auto& historyComponent = world.WriteComponent<editor::inspector::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (const auto& request : world.Events<editor::inspector::HistoryUndoEvent>())
	{
		auto& historyComponent = world.WriteComponent<editor::inspector::HistoryComponent>();
		if (historyComponent.m_UndoStack.IsEmpty())
			continue;

		// apply the change
		HistoryData& history = historyComponent.m_UndoStack.GetLast();
		core::VariantMatch(history, [&](auto&& data) { Undo(m_World, data); });

		// move from undo -> redo
		historyComponent.m_RedoStack.Append(std::move(history));
		historyComponent.m_UndoStack.Pop();
	}

	for (const auto& request : world.Events<editor::inspector::HistoryRedoEvent>())
	{
		auto& historyComponent = world.WriteComponent<editor::inspector::HistoryComponent>();
		if (historyComponent.m_RedoStack.IsEmpty())
			continue;

		// apply the change
		HistoryData& history = historyComponent.m_RedoStack.GetLast();
		core::VariantMatch(history, [&](auto&& data) { Redo(m_World, data); });

		// move from redo -> undo
		historyComponent.m_UndoStack.Append(std::move(history));
		historyComponent.m_RedoStack.Pop();
	}
}