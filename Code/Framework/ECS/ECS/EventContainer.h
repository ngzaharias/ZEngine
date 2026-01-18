#pragma once

#include "Core/Array.h"
#include "Core/MemBuffer.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace ecs
{
	class IEventContainer
	{
		friend class EntityWorld;

	public:
		virtual ~IEventContainer() = default;

		virtual int32 GetCount() const = 0;

		virtual void MoveAll(IEventContainer& destination) = 0;

		virtual void ReadAt(MemBuffer& buffer, int32 index) = 0;
		virtual void ReadAll(MemBuffer& buffer) = 0;

		virtual void RemoveAll() = 0;
	};

	template<typename TEvent>
	class EventContainer : public IEventContainer
	{
		friend class EntityWorld;

	public:
		~EventContainer() override = default;

		inline Array<TEvent>& GetValues();
		inline const Array<TEvent>& GetValues() const;

		template<typename... TArgs>
		inline TEvent& Emplace(TArgs&& ...args);

		inline int32 GetCount() const;

		inline void MoveAll(IEventContainer& destination) override;

		inline void ReadAt(MemBuffer& buffer, int32 index) override;
		inline void ReadAll(MemBuffer& buffer) override;

		inline void RemoveAll() override;

	private:
		Array<TEvent> m_Data = { };
	};
}

#include "EventContainer.inl"