
template<class TComponent>
void ecs::EntityStorage::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_AliveComponents.Set(componentId, new ecs::ComponentContainer<TComponent>());
	m_DeadComponents.Set(componentId, new ecs::ComponentContainer<TComponent>());
	m_EntityBuffer.RegisterComponent<TComponent>();
}

template <typename TComponent, typename... TArgs>
auto ecs::EntityStorage::AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&
{
	return m_EntityBuffer.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void ecs::EntityStorage::RemoveComponent(const ecs::Entity& entity)
{
	m_EntityBuffer.RemoveComponent<TComponent>(entity);
}

template<class TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	const ecs::IComponentContainer* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	return istorage->Contains(entity);
}

template<class TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent&
{
	using Storage = ecs::ComponentContainer<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentContainer* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Get(entity);
}

template<class TComponent>
auto ecs::EntityStorage::TryComponent(const ecs::Entity& entity, const bool alive /*= true*/) const -> TComponent*
{
	using Storage = ecs::ComponentContainer<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentContainer* istorage = alive
		? m_AliveComponents.Get(componentId)
		: m_DeadComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Try(entity);
}