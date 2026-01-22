
template<class TComponent>
void ecs::EntityBuffer::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_AddedComponents.Set(componentId, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent, typename... TArgs>
auto ecs::EntityBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Added.Raise(componentId);

	ecs::IComponentStorage* istorage = m_AddedComponents.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityBuffer::RemoveComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Removed.Raise(componentId);
}

template<class TComponent>
void ecs::EntityBuffer::UpdateComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Updated.Raise(componentId);
}