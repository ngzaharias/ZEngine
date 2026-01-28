#pragma once

#include "Core/Array.h"
#include "Core/MemBuffer.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace ecs
{
	class IEventContainer
	{
		friend class EventStorage;

	public:
		virtual ~IEventContainer() = default;

		virtual int32 GetCount() const = 0;

		virtual void MoveAll(IEventContainer& destination) = 0;

		virtual void ReadAt(MemBuffer& buffer, int32 index) const = 0;
		virtual void ReadAll(MemBuffer& buffer) const = 0;

		virtual void RemoveAll() = 0;
	};

	template<typename TEvent>
	class EventContainer : public IEventContainer
	{
		friend class EventStorage;

	public:
		~EventContainer() override = default;

		template<typename... TArgs>
		inline TEvent& Emplace(TArgs&& ...args);

		inline int32 GetCount() const;

		inline const Array<TEvent>& GetValues() const;

		inline void MoveAll(IEventContainer& destination) override;

		inline void ReadAt(MemBuffer& buffer, int32 index) const override;
		inline void ReadAll(MemBuffer& buffer) const override;

		inline void RemoveAll() override;

	private:
		Array<TEvent> m_Data = { };
	};
}

#include "EventContainer.inl"