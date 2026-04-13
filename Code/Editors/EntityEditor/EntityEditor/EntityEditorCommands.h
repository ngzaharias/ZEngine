#pragma once

#include "Core/Guid.h"
#include "Core/Hash.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "Engine/UUIDHelpers.h"

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
		virtual bool Merge(Command& command) { return false; }

		int32 m_Command = 0;
		int32 m_TypeId = 0;
		float m_Time = 0.f;
		bool m_Merge = false;
	};

	template<typename TComponent, typename... TArgs>
	struct ComponentAdd : public Command
	{
	public:
		ComponentAdd(const str::Guid& uuid, TArgs... args)
			: m_UUID(uuid)
			, m_Args(std::forward<TArgs>(args)...)
		{ }

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_UUID = {};
		std::tuple<TArgs...> m_Args = {};
	};

	template<typename TComponent>
	struct ComponentRemove : public Command
	{
	public:
		ComponentRemove(const str::Guid& uuid, const TComponent& data)
			: m_UUID(uuid)
			, m_Data(data)
		{ }

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_UUID = {};
		TComponent m_Data = {};
	};

	template<typename TComponent, typename TValue>
	struct ComponentField : public Command
	{
		using TField = TValue TComponent::*;

	public:
		ComponentField(TField fieldPtr, const str::Guid& uuid, const TValue& valueOld, const TValue& valueNew)
			: m_FieldPtr(fieldPtr)
			, m_UUID(uuid)
			, m_ValueOld(valueOld)
			, m_ValueNew(valueNew)
		{ 
			m_Command = 1;
			m_TypeId = ToTypeId<TComponent>();
		}

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;
		bool Merge(Command& command) override;

	private:
		TField m_FieldPtr = nullptr;

		str::Guid m_UUID = {};
		TValue m_ValueOld = {};
		TValue m_ValueNew = {};
	};

	template<typename TComponent>
	struct ComponentUpdate : public Command
	{
	public:
		ComponentUpdate(const str::Guid& uuid, const TComponent& valueOld, const TComponent& valueNew)
			: m_UUID(uuid)
			, m_ValueOld(valueOld)
			, m_ValueNew(valueNew)
		{ 
			m_Command = 2;
			m_TypeId = ToTypeId<TComponent>();
		}

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;
		bool Merge(Command& command) override;

	private:
		str::Guid m_UUID = {};
		TComponent m_ValueOld = {};
		TComponent m_ValueNew = {};
	};

	struct EntityCreate : public Command
	{
	public:
		EntityCreate(const str::Guid& uuid, const str::StringView& name);

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_UUID = {};
		str::String m_Name = {};
	};

	struct EntityDestroy : public Command
	{
	public:
		EntityDestroy(const str::Guid& uuid);
		EntityDestroy(const str::Guid& uuid, str::String&& data);
		EntityDestroy(const str::Guid& uuid, const str::String& data);

		const char* ToString() const override;

		void Exec(ecs::EntityWorld& world) override;
		void Undo(ecs::EntityWorld& world) override;

	private:
		str::Guid m_UUID = {};
		str::String m_Data = {};
	};
}

#include "EntityEditorCommands.inl"