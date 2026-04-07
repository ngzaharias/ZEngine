#pragma once

#include "Command/Command.h"
#include "Core/Guid.h"
#include "Core/String.h"

struct EntityCreate : public Command
{
public:
	EntityCreate(const str::String& name);

	const char* ToString() const override
	{
		return "EntityCreate";
	}

	void Exec(ecs::EntityWorld& world) override;
	void Redo(ecs::EntityWorld& world) override;
	void Undo(ecs::EntityWorld& world) override;

private:
	str::Guid m_Guid = {};
	str::String m_Name = {};
};
