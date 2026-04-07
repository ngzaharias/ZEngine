#pragma once

#include "Command/Command.h"
#include "Core/Guid.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/TemplateHelpers.h"

template<typename TComponent>
struct ComponentRemove : public Command
{
public:
	ComponentRemove(const str::Guid& guid, const TComponent& data)
		: m_Guid(guid)
		, m_Data(data)
	{ }

	const char* ToString() const override
	{
		return "ComponentRemove";
	}

	void Exec(ecs::EntityWorld& world) override;
	void Redo(ecs::EntityWorld& world) override;
	void Undo(ecs::EntityWorld& world) override;

private:
	str::Guid m_Guid = {};
	TComponent m_Data = {};
};

#include "ComponentRemove.inl"