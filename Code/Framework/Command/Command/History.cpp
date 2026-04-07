#include "Command/History.h"

#include "ECS/EntityWorld.h"

History::History(ecs::EntityWorld& world)
	: m_World(world)
{
}

void History::Undo()
{
	if (m_UndoStack.IsEmpty())
		return;

	// apply the change
	Command* command = m_UndoStack.GetLast();
	command->Undo(m_World);

	// move from undo -> redo
	m_RedoStack.Append(command);
	m_UndoStack.Pop();
}

void History::Redo()
{
	if (m_RedoStack.IsEmpty())
		return;

	// apply the change
	Command* command = m_RedoStack.GetLast();
	command->Redo(m_World);

	// move from redo -> undo
	m_UndoStack.Append(command);
	m_RedoStack.Pop();
}

bool History::HasUndo() const
{
	return !m_UndoStack.IsEmpty();
}

bool History::HasRedo() const
{
	return !m_RedoStack.IsEmpty();
}

const Array<Command*>& History::GetUndos() const
{
	return m_UndoStack;
}

const Array<Command*>& History::GetRedos() const
{
	return m_RedoStack;
}

