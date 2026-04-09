#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/TemplateHelpers.h"

#include <tuple>

namespace editor::entity
{
	struct Command
	{
		virtual ~Command() = default;

		virtual const char* ToString() const = 0;

		virtual void Exec(ecs::EntityWorld& world) = 0;
		virtual void Redo(ecs::EntityWorld& world) { Exec(world); }
		virtual void Undo(ecs::EntityWorld& world) = 0;
	};

	template<typename TComponent, typename... TArgs>
	struct ComponentAdd : public Command
	{
	public:
		ComponentAdd(const str::Guid& guid, TArgs... args)
			: m_Guid(guid)
			, m_Args(std::forward<TArgs>(args)...)
		{ }

		const char* ToString() const override
		{
			return "ComponentAdd";
		}

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_Guid = {};
		std::tuple<TArgs...> m_Args = {};
	};

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
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_Guid = {};
		TComponent m_Data = {};
	};

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
		void Undo(ecs::EntityWorld& world) override;

	private:
		TField m_FieldPtr = nullptr;

		str::Guid m_Guid = {};
		TValue m_ValueOld = {};
		TValue m_ValueNew = {};
	};

	struct EntityCreate : public Command
	{
	public:
		EntityCreate(const str::String& name);

		const char* ToString() const override
		{
			return "EntityCreate";
		}

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_Guid = {};
		str::String m_Name = {};
	};

	struct EntityDestroy : public Command
	{
	public:
		EntityDestroy(const str::Guid& entity);
		EntityDestroy(const str::Guid& entity, str::String&& data);
		EntityDestroy(const str::Guid& entity, const str::String& data);

		const char* ToString() const override
		{
			return "EntityDestroy";
		}

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_Entity = {};
		str::String m_Data = {};
	};
}

#include "EntityEditorCommands.inl"