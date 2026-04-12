#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorCommandManager.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/TemplateManager.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDHelpers.h"

editor::entity::CommandManager::CommandManager(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::entity::CommandManager::ExecuteCommands()
{
	if (m_ExecStack.IsEmpty())
		return;

	for (Command* command : m_ExecStack)
		command->Exec(m_World);

	m_UndoStack.Append(m_ExecStack);
	m_ExecStack.RemoveAll();
	m_RedoStack.RemoveAll();
}

void editor::entity::CommandManager::UndoLastCommand()
{
	if (m_UndoStack.IsEmpty())
		return;

	// apply the change
	editor::entity::Command* command = m_UndoStack.GetLast();
	command->Undo(m_World);

	// move from undo -> redo
	m_RedoStack.Append(command);
	m_UndoStack.Pop();
}

void editor::entity::CommandManager::RedoLastCommand()
{
	if (m_RedoStack.IsEmpty())
		return;

	// apply the change
	editor::entity::Command* command = m_RedoStack.GetLast();
	command->Redo(m_World);

	// move from redo -> undo
	m_UndoStack.Append(command);
	m_RedoStack.Pop();
}

bool editor::entity::CommandManager::HasUndoCommand() const
{
	return !m_UndoStack.IsEmpty();
}

bool editor::entity::CommandManager::HasRedoCommand() const
{
	return !m_RedoStack.IsEmpty();
}

const Array<editor::entity::Command*>& editor::entity::CommandManager::GetUndoCommands() const
{
	return m_UndoStack;
}

const Array<editor::entity::Command*>& editor::entity::CommandManager::GetRedoCommands() const
{
	return m_RedoStack;
}

void editor::entity::CommandManager::AddCommands(Array<Command*>&& commands)
{
	m_ExecStack.Append(commands);
}

void editor::entity::CommandManager::EntityCreate(const str::Guid& uuid, const str::StringView& name)
{
	m_ExecStack.Append(new editor::entity::EntityCreate(uuid, name));
}

void editor::entity::CommandManager::EntityDestroy(const str::Guid& uuid)
{
	Visitor visitor;
	const ecs::Entity entity = eng::ToEntity(m_World, uuid);

	const auto& manager = m_World.ReadResource<eng::TemplateManager>();
	manager.ReadEntity(m_World, entity, visitor);

	str::String data = visitor;
	m_ExecStack.Append(new editor::entity::EntityDestroy(uuid, std::move(data)));
}

