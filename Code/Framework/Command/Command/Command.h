#pragma once

namespace ecs
{
	class EntityWorld;
}

struct Command
{
	virtual ~Command() = default;

	virtual const char* ToString() const = 0;

	virtual void Exec(ecs::EntityWorld& world) = 0;
	virtual void Redo(ecs::EntityWorld& world) = 0;
	virtual void Undo(ecs::EntityWorld& world) = 0;
};