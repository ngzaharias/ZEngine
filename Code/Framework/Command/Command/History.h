#pragma once

#include "Command/Command.h"
#include "Core/Array.h"

namespace ecs
{
	class EntityWorld;
}

class History
{
public:
	History(ecs::EntityWorld& world);

	void Undo();
	void Redo();

	bool HasUndo() const;
	bool HasRedo() const;

	const Array<Command*>& GetUndos() const;
	const Array<Command*>& GetRedos() const;

protected:
	ecs::EntityWorld& m_World;
	Array<Command*> m_RedoStack = {};
	Array<Command*> m_UndoStack = {};
};
