#pragma once

template<class TComponent>
bool ut::RAIIHelper::HasComponent(const ecs::Entity& entity) const
{
	return m_EntityWorld.HasComponent<TComponent>(entity);
}

template<class TComponent>
auto ut::RAIIHelper::GetComponent(const ecs::Entity& entity)->TComponent&
{
	return m_EntityWorld.GetComponent<TComponent>(entity);
}

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