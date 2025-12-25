#pragma once

#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentStorage.h"
#include "ECS/ComponentTag.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "ECS/EventStorage.h"
#include "ECS/Singleton.h"
#include "ECS/SingletonStorage.h"

namespace ecs
{
	class IEventStorage
	{
		friend class EntityWorld;

	public:
		virtual ~IEventStorage() = default;

		virtual void Move(IEventStorage& destination) = 0;
		virtual void RemoveAll() = 0;
	};

	template<typename TEvent>
	class EventStorage : public IEventStorage
	{
		friend class EntityWorld;

	public:
		~EventStorage() override = default;

		inline Array<TEvent>& GetValues();
		inline const Array<TEvent>& GetValues() const;

		template<typename... TArgs>
		inline TEvent& Emplace(TArgs&& ...args);

		inline void Move(IEventStorage& destination) override;

		inline void RemoveAll() override;

	private:
		Array<TEvent> m_Data = { };
	};
}

#include "EventStorage.inl"