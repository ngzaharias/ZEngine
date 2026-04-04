#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorHistorySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"
#include "Engine/TemplateMapComponent.h"
#include "EntityEditor/EntityEditorEntityCreatedEvent.h"
#include "EntityEditor/EntityEditorEntityDestroyedEvent.h"
#include "EntityEditor/EntityEditorEntitySelectedEvent.h"
#include "EntityEditor/EntityEditorEntityUpdatedEvent.h"
#include "EntityEditor/EntityEditorHistoryComponent.h"
#include "EntityEditor/EntityEditorHistoryRedoEvent.h"
#include "EntityEditor/EntityEditorHistoryUndoEvent.h"
#include "EntityEditor/EntityEditorSelectComponent.h"

namespace 
{
	void Undo(ecs::EntityWorld& world, editor::entity::EntityCreated& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);
		world.DestroyEntity(entity);
	}

	void Redo(ecs::EntityWorld& world, editor::entity::EntityCreated& command)
	{
		const ecs::Entity entity = world.CreateEntity();
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.LoadEntity(world, entity, command.m_Data);

		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = entity;
	}

	void Undo(ecs::EntityWorld& world, editor::entity::EntityDestroyed& command)
	{
		const ecs::Entity entity = world.CreateEntity();
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.LoadEntity(world, entity, command.m_Data);

		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = entity;
	}

	void Redo(ecs::EntityWorld& world, editor::entity::EntityDestroyed& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);
		world.DestroyEntity(entity);
	}

	void Undo(ecs::EntityWorld& world, editor::entity::EntityUpdated& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);

		str::String redoData;
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(world, entity, redoData);
		manager.SyncEntity(world, entity, command.m_Data);
		command.m_Data = std::move(redoData);

		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = entity;
	}

	void Redo(ecs::EntityWorld& world, editor::entity::EntityUpdated& command)
	{
		const ecs::Entity entity = eng::ToEntity(world, command.m_Guid);

		str::String undoData;
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(world, entity, undoData);
		manager.SyncEntity(world, entity, command.m_Data);
		command.m_Data = std::move(undoData);

		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = entity;
	}

	void Undo(ecs::EntityWorld& world, editor::entity::EntitySelected& command)
	{
		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = eng::ToEntity(world, command.m_Before);
	}

	void Redo(ecs::EntityWorld& world, editor::entity::EntitySelected& command)
	{
		auto& select = world.WriteComponent<editor::entity::SelectComponent>();
		select.m_Entity = eng::ToEntity(world, command.m_After);
	}
}

editor::entity::HistorySystem::HistorySystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::entity::HistorySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& request : world.Events<editor::entity::EntityCreatedEvent>())
	{
		EntityCreated command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(m_World, request.m_Entity, command.m_Data);

		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (const auto& request : world.Events<editor::entity::EntityDestroyedEvent>())
	{
		EntityDestroyed command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		const auto& manager = world.ReadResource<eng::TemplateManager>();
		manager.ReadEntity(m_World, request.m_Entity, command.m_Data);

		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (auto& request : world.Events<editor::entity::EntitySelectedEvent>())
	{
		EntitySelected command;
		command.m_After = eng::ToGuid(m_World, request.m_After);
		command.m_Before = eng::ToGuid(m_World, request.m_Before);

		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (auto& request : world.Events<editor::entity::EntityUpdatedEvent>())
	{
		EntityUpdated command;
		command.m_Guid = eng::ToGuid(m_World, request.m_Entity);
		command.m_Data = request.m_Data;

		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
		historyComponent.m_RedoStack.RemoveAll();
		historyComponent.m_UndoStack.Append(std::move(command));
	}

	for (const auto& request : world.Events<editor::entity::HistoryUndoEvent>())
	{
		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
		if (historyComponent.m_UndoStack.IsEmpty())
			continue;

		// apply the change
		HistoryData& history = historyComponent.m_UndoStack.GetLast();
		core::VariantMatch(history, [&](auto&& data) { Undo(m_World, data); });

		// move from undo -> redo
		historyComponent.m_RedoStack.Append(std::move(history));
		historyComponent.m_UndoStack.Pop();
	}

	for (const auto& request : world.Events<editor::entity::HistoryRedoEvent>())
	{
		auto& historyComponent = world.WriteComponent<editor::entity::HistoryComponent>();
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