#pragma once

#include "Command/Command.h"
#include "Core/Guid.h"
#include "Core/String.h"

struct EntityDestroy : public Command
{
public:
	EntityDestroy(const str::Guid& entity);
	EntityDestroy(const str::Guid& entity, str::String&& data);
	EntityDestroy(const str::Guid& entity, const str::String& data);

	void Exec(ecs::EntityWorld& world) override;
	void Redo(ecs::EntityWorld& world) override;
	void Undo(ecs::EntityWorld& world) override;

private:
	str::Guid m_Entity = {};
	str::String m_Data = {};
};
