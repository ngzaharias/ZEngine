#pragma once

#include "Command/Command.h"
#include "Command/ComponentAdd.h"
#include "Command/ComponentRemove.h"
#include "Command/ComponentUpdate.h"
#include "Command/History.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "ECS/EntityWorld.h"

namespace editor::entity
{
	class Commands final : public History
	{
	public:
		Commands(ecs::EntityWorld& world);

		void Update();

		//////////////////////////////////////////////////////////////////////////
		// Entity

		void CreateEntity(const str::StringView& name);
		
		void DestroyEntity(const str::Guid& guid);
		
		//////////////////////////////////////////////////////////////////////////
		// Component

		template<typename TComponent, typename... TArgs>
		void AddComponent(const str::Guid& guid, TArgs&&... args);

		template<typename TComponent>
		void RemoveComponent(const str::Guid& guid);

		template <typename TComponent, typename TValue>
		void UpdateComponent(TValue TComponent::* fieldPtr, const str::Guid& guid, const TValue& valueOld, const TValue& valueNew);

	private:
		Array<Command*> m_ExecStack = {};
	};
}

#include "EntityEditorCommands.inl"