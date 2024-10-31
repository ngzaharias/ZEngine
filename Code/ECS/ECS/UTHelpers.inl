#pragma once

template<typename TComponent>
auto ut::RAIIHelper::AddComponent(const ecs::Entity& entity)->TComponent&
{
	return m_EntityWorld.AddComponent<TComponent>(entity);
}

template<typename TComponent>
void ut::RAIIHelper::RemoveComponent(const ecs::Entity& entity)
{
	return m_EntityWorld.RemoveComponent<TComponent>(entity);
}

template<class TComponent>
bool ut::RAIIHelper::HasComponent(const ecs::Entity& entity) const
{
	return m_EntityWorld.HasComponent<TComponent>(entity);
}

template<class TComponent>
auto ut::RAIIHelper::ReadComponent(const ecs::Entity& entity)->const TComponent&
{
	return m_EntityWorld.ReadComponent<TComponent>(entity);
}

template<class TComponent>
auto ut::RAIIHelper::WriteComponent(const ecs::Entity& entity)->TComponent&
{
	return m_EntityWorld.WriteComponent<TComponent>(entity);
}