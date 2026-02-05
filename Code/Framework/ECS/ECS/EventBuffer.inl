
template<class TEvent>
bool ecs::EventBuffer::IsRegistered() const
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	return m_Containers.Contains(typeId);
}

template<class TEvent>
void ecs::EventBuffer::RegisterEvent()
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	m_Containers.Set(typeId, new ecs::EventContainer<TEvent>());
}

template<class TEvent, typename... TArgs>
auto ecs::EventBuffer::AddEvent(TArgs&&... args) -> TEvent&
{
	ecs::EventContainer<TEvent>& container = GetAt<TEvent>();
	return container.Emplace(std::forward<TArgs>(args)...);
}

template<typename TEvent>
ecs::EventContainer<TEvent>& ecs::EventBuffer::GetAt()
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	ecs::IEventContainer& container = GetAt(typeId);
	return static_cast<ecs::EventContainer<TEvent>&>(container);
}

template<typename TEvent>
const ecs::EventContainer<TEvent>& ecs::EventBuffer::GetAt() const
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	const ecs::IEventContainer& container = GetAt(typeId);
	return static_cast<const ecs::EventContainer<TEvent>&>(container);
}

template<typename TEvent>
ecs::EventContainer<TEvent>* ecs::EventBuffer::TryAt()
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	ecs::IEventContainer* container = TryAt(typeId);
	return static_cast<ecs::EventContainer<TEvent>*>(container);
}

template<typename TEvent>
const ecs::EventContainer<TEvent>* ecs::EventBuffer::TryAt() const
{
	const ecs::EventId typeId = ToTypeId<TEvent, ecs::EventTag>();
	const ecs::IEventContainer* container = TryAt(typeId);
	return static_cast<const ecs::EventContainer<TEvent>*>(container);
}