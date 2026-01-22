
template<class TComponent>
void ecs::EntityStorage::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_AliveComponents.Set(componentId, new ecs::ComponentStorage<TComponent>());
	m_DeadComponents.Set(componentId, new ecs::ComponentStorage<TComponent>());
	m_Buffer.RegisterComponent<TComponent>();
}

template<class TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	const ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	return istorage->Contains(entity);
}

template<class TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent&
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Get(entity);
}

template<class TComponent>
auto ecs::EntityStorage::TryComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent*
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentStorage* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Try(entity);
}