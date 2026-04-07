#pragma once

#include "Command/Command.h"
#include "Core/Guid.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/TemplateHelpers.h"

#include <tuple>

template<typename TComponent, typename... TArgs>
struct ComponentAdd : public Command
{
public:
	ComponentAdd(const str::Guid& guid, TArgs... args)
		: m_Guid(guid)
		, m_Args(std::forward<TArgs>(args)...)
	{ }

	void Exec(ecs::EntityWorld& world) override;
	void Redo(ecs::EntityWorld& world) override;
	void Undo(ecs::EntityWorld& world) override;

private:
	str::Guid m_Guid = {};
	std::tuple<TArgs...> m_Args = {};
};

#include "ComponentAdd.inl"