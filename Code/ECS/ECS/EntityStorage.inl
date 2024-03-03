#pragma once

template<class TComponent>
bool ecs::EntityStorage::IsRegistered() const
{
	using NonConst = std::remove_const<TComponent>::type;

	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_AliveComponents.Contains(componentId);
}

template<class TComponent>
void ecs::EntityStorage::RegisterComponent()
{
	using NonConst = std::remove_const<TComponent>::type;
	using Storage = ecs::ComponentStorage<NonConst>;

	const ecs::ComponentId componentId = ToTypeIndex<NonConst, ecs::ComponentTag>();
	m_AliveComponents.Set(componentId, new Storage());
	m_DeadComponents.Set(componentId, new Storage());
}

template<class TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	using NonConst = std::remove_const<TComponent>::type;

	const ecs::ComponentId componentId = ToTypeIndex<NonConst, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	return istorage->Contains(entity);
}

template<class TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/)->TComponent&
{
	using NonConst = std::remove_const<TComponent>::type;
	using Storage = ecs::ComponentStorage<NonConst>;

	const ecs::ComponentId componentId = ToTypeIndex<NonConst, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Get(entity);
}