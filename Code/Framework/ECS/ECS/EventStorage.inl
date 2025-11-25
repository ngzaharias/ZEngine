#pragma once

template<typename TEvent>
inline Array<TEvent>& ecs::EventStorage<TEvent>::GetValues()
{
	return m_Data;
}

template<typename TEvent>
inline const Array<TEvent>& ecs::EventStorage<TEvent>::GetValues() const
{
	return m_Data;
}

template<typename TEvent>
template<typename... TArgs>
inline TEvent& ecs::EventStorage<TEvent>::Emplace(TArgs&& ...args)
{
	return m_Data.Emplace(std::forward<TArgs>(args)...);
}

template<typename TEvent>
inline void ecs::EventStorage<TEvent>::Move(IEventStorage& destination)
{
	auto& storage = static_cast<ecs::EventStorage<TEvent>&>(destination);
	storage.m_Data = std::move(m_Data);
}

template<typename TEvent>
inline void ecs::EventStorage<TEvent>::RemoveAll()
{
	m_Data.RemoveAll();
}