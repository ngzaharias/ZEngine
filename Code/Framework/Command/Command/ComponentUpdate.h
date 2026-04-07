#pragma once

#include "Command/Command.h"
#include "Core/Guid.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/TemplateHelpers.h"

template<typename TComponent, typename TValue>
struct ComponentUpdate : public Command
{
	using TField = TValue TComponent::*;

public:
	ComponentUpdate(TField fieldPtr, const str::Guid& guid, const TValue& valueOld, const TValue& valueNew)
		: m_FieldPtr(fieldPtr)
		, m_Guid(guid)
		, m_ValueOld(valueOld)
		, m_ValueNew(valueNew)
	{ }

	const char* ToString() const override
	{
		return "ComponentUpdate";
	}

	void Exec(ecs::EntityWorld& world) override;
	void Redo(ecs::EntityWorld& world) override;
	void Undo(ecs::EntityWorld& world) override;

private:
	TField m_FieldPtr = nullptr;

	str::Guid m_Guid = {};
	TValue m_ValueOld = {};
	TValue m_ValueNew = {};
};

#include "ComponentUpdate.inl"