#pragma once

template<class TComponent>
void ecs::EntityStorage::RegisterComponent()
{
	const ecs::ComponentId id = ToTypeId<TComponent, ecs::ComponentTag>();
	m_AliveComponents.Set(id, new ecs::ComponentStorage<TComponent>());
	m_DeadComponents.Set(id, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	const ecs::ComponentId id = ToTypeId<TComponent, ecs::ComponentTag>();
	const ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(id)
		: m_DeadComponents.Get(id);
	return istorage->Contains(entity);
}

template<class TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent&
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId id = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(id)
		: m_DeadComponents.Get(id);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Get(entity);
}

template<class TComponent>
auto ecs::EntityStorage::TryComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent*
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId id = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(id)
		: m_DeadComponents.Get(id);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Try(entity);
}

template<class TEvent>
void ecs::EntityStorage::RegisterEvent()
{
	const ecs::EventId id = ToTypeId<TEvent, ecs::EventTag>();
	m_Events.Set(id, new ecs::EventStorage<TEvent>());
}

template<class TEvent>
auto ecs::EntityStorage::GetEvents() const -> const Array<TEvent>&
{
	using Storage = ecs::EventStorage<TEvent>;

	const ecs::EventId id = ToTypeId<TEvent, ecs::EventTag>();
	const ecs::IEventStorage* istorage = m_Events.Get(id);
	const Storage* storage = static_cast<const Storage*>(istorage);
	return storage->GetValues();
}

template<class TSingleton, typename... TArgs>
void ecs::EntityStorage::RegisterSingleton(TArgs&&... args)
{
	const ecs::SingletonId id = ToTypeId<TSingleton, ecs::SingletonTag>();
	m_Singletons.Set(id, new ecs::SingletonStorage<TSingleton>(std::forward<TArgs>(args)...));
}

template<class TSingleton>
auto ecs::EntityStorage::GetSingleton() -> TSingleton&
{
	using Storage = ecs::SingletonStorage<TSingleton>;

	const ecs::SingletonId id = ToTypeId<TSingleton, ecs::SingletonTag>();
	ecs::ISingletonStorage* istorage = m_Singletons.Get(id);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->GetData();
}
