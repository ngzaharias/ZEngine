#pragma once

template<typename TComponent>
inline bool ecs::ComponentStorage<TComponent>::Contains(const ecs::Entity& entity) const
{
	return m_Data.Contains(entity);
}

template<typename TComponent>
inline TComponent& ecs::ComponentStorage<TComponent>::Get(const ecs::Entity& entity)
{
	return m_Data.Get(entity);
}

template<typename TComponent>
inline const TComponent& ecs::ComponentStorage<TComponent>::Get(const ecs::Entity& entity) const
{
	return m_Data.Get(entity);
}

template<typename TComponent>
inline Array<TComponent>& ecs::ComponentStorage<TComponent>::GetValues()
{
	return m_Data.GetValues();
}

template<typename TComponent>
inline const Array<TComponent>& ecs::ComponentStorage<TComponent>::GetValues() const
{
	return m_Data.GetValues();
}

template<typename TComponent>
template<typename... TArgs>
inline TComponent& ecs::ComponentStorage<TComponent>::Emplace(const ecs::Entity& entity, TArgs&& ...args)
{
	return m_Data.Emplace(entity, std::forward<TArgs>(args)...);
}

template<typename TComponent>
inline TComponent& ecs::ComponentStorage<TComponent>::Set(const ecs::Entity& entity, TComponent&& value)
{
	return m_Data.Set(entity, std::move(value));
}

template<typename TComponent>
inline TComponent& ecs::ComponentStorage<TComponent>::Set(const ecs::Entity& entity, const TComponent& value)
{
	return m_Data.Set(entity, value);
}

template<typename TComponent>
inline void ecs::ComponentStorage<TComponent>::Move(IComponentStorage& destination)
{
	auto& storage = static_cast<ecs::ComponentStorage<TComponent>&>(destination);
	for (auto&& [entity, component] : m_Data)
	{
		Z_PANIC(!storage.Contains(entity), "Trying to add component to an entity that already has it!");
		storage.Set(entity, std::move(component));
	}
	RemoveAll();
}

template<typename TComponent>
inline void ecs::ComponentStorage<TComponent>::Move(const ecs::Entity& entity, IComponentStorage& destination)
{
	auto& storage = static_cast<ecs::ComponentStorage<TComponent>&>(destination);
	Z_PANIC(!storage.Contains(entity), "Trying to add component to an entity that already has it!");

	TComponent& component = Get(entity);
	storage.Set(entity, std::move(component));

	Remove(entity);
}

template<typename TComponent>
inline void ecs::ComponentStorage<TComponent>::Remove(const Entity& entity)
{
	m_Data.Remove(entity);
}

template<typename TComponent>
inline void ecs::ComponentStorage<TComponent>::RemoveAll()
{
	m_Data.RemoveAll();
}