#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/StringView.h"
#include "ECS/EntityWorld.h"
#include "EntityEditor/EntityEditorCommands.h"

#include <tuple>

namespace ecs
{
	class EntityWorld;
}

namespace editor::entity
{
	class CommandManager final
	{
	public:
		CommandManager(ecs::EntityWorld& world);

		void ExecuteCommands();
		void UndoLastCommand();
		void RedoLastCommand();

		bool HasUndoCommand() const;
		bool HasRedoCommand() const;

		const Array<Command*>& GetUndoCommands() const;
		const Array<Command*>& GetRedoCommands() const;

		//////////////////////////////////////////////////////////////////////////
		// Command

		void AddCommands(Array<Command*>&& commands);

		//////////////////////////////////////////////////////////////////////////
		// Entity

		void CreateEntity(const str::Guid& uuid, const str::StringView& name);
		
		void DestroyEntity(const str::Guid& uuid);
		
		//////////////////////////////////////////////////////////////////////////
		// Component

		template<typename TComponent, typename... TArgs>
		void AddComponent(const str::Guid& guid, TArgs&&... args);

		template<typename TComponent>
		void RemoveComponent(const str::Guid& guid);

		template <typename TComponent, typename TValue>
		void UpdateComponent(TValue TComponent::* fieldPtr, const str::Guid& guid, const TValue& valueOld, const TValue& valueNew);

	private:
		ecs::EntityWorld& m_World;
		Array<Command*> m_ExecStack = {};
		Array<Command*> m_RedoStack = {};
		Array<Command*> m_UndoStack = {};
	};
}

#include "EntityEditorCommandManager.inl"