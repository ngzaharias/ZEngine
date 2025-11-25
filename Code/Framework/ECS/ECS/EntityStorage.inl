#pragma once

template<class TComponent>
void ecs::EntityStorage::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_AliveComponents.Set(componentId, new ecs::ComponentStorage<TComponent>());
	m_DeadComponents.Set(componentId, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	const ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	return istorage->Contains(entity);
}

template<class TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/) -> TComponent&
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Get(entity);
}

template<class TEvent>
void ecs::EntityStorage::RegisterEvent()
{
	const ecs::EventId eventId = ToTypeIndex<TEvent, ecs::EventTag>();
	m_Events.Set(eventId, new ecs::EventStorage<TEvent>());
}

template<class TEvent>
auto ecs::EntityStorage::GetEvents() const -> const Array<TEvent>&
{
	using Storage = ecs::EventStorage<TEvent>;

	const ecs::EventId eventId = ToTypeIndex<TEvent, ecs::EventTag>();
	const ecs::IEventStorage* istorage = m_Events.Get(eventId);
	const Storage* storage = static_cast<const Storage*>(istorage);
	return storage->GetValues();
}
