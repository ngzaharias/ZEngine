
template<typename TEvent>
inline const Array<TEvent>& ecs::EventContainer<TEvent>::GetValues() const
{
	return m_Data;
}

template<typename TEvent>
template<typename... TArgs>
inline TEvent& ecs::EventContainer<TEvent>::Emplace(TArgs&& ...args)
{
	return m_Data.Emplace(std::forward<TArgs>(args)...);
}

template<typename TEvent>
inline int32 ecs::EventContainer<TEvent>::GetCount() const
{
	return m_Data.GetCount();
}

template<typename TEvent>
inline void ecs::EventContainer<TEvent>::MoveAll(IEventContainer& destination)
{
	auto& container = static_cast<ecs::EventContainer<TEvent>&>(destination);
	container.m_Data.Append(std::move(m_Data));
	m_Data.RemoveAll();
}

template<typename TEvent>
inline void ecs::EventContainer<TEvent>::ReadAt(MemBuffer& buffer, int32 index) const
{
	if constexpr (std::is_base_of<ecs::IsReplicated, TEvent>::value)
	{
		buffer.Write(m_Data[index]);
	}
}

template<typename TEvent>
inline void ecs::EventContainer<TEvent>::ReadAll(MemBuffer& buffer) const
{
	if constexpr (std::is_base_of<ecs::IsReplicated, TEvent>::value)
	{
		buffer.Write(m_Data.GetCount());
		for (const TEvent& data : m_Data)
			buffer.Write(data);
	}
}

template<typename TEvent>
inline void ecs::EventContainer<TEvent>::RemoveAll()
{
	m_Data.RemoveAll();
}