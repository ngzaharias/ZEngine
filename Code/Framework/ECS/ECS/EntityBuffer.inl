
//////////////////////////////////////////////////////////////////////////
// Component

template<class TComponent>
void ecs::EntityBuffer::RegisterComponent()
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_Components.Set(typeId, new ecs::ComponentContainer<TComponent>());
}

template<class TComponent, typename... TArgs>
auto ecs::EntityBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Added.Raise(typeId);

	ecs::ComponentContainer<TComponent>& container = GetContainerAt<TComponent>();
	return container.Emplace(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityBuffer::UpdateComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Updated.Raise(typeId);
}

template<class TComponent>
void ecs::EntityBuffer::RemoveComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Removed.Raise(typeId);
}

//////////////////////////////////////////////////////////////////////////
// Container

template<typename TComponent>
ecs::ComponentContainer<TComponent>& ecs::EntityBuffer::GetContainerAt()
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentContainer& container = GetContainerAt(typeId);
	return static_cast<ecs::ComponentContainer<TComponent>&>(container);
}

template<typename TComponent>
const ecs::ComponentContainer<TComponent>& ecs::EntityBuffer::GetContainerAt() const
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	const ecs::IComponentContainer& container = GetContainerAt(typeId);
	return static_cast<const ecs::ComponentContainer<TComponent>&>(container);
}

template<typename TComponent>
ecs::ComponentContainer<TComponent>* ecs::EntityBuffer::TryContainerAt()
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	ecs::IComponentContainer* container = TryContainerAt(typeId);
	return static_cast<ecs::ComponentContainer<TComponent>*>(container);
}

template<typename TComponent>
const ecs::ComponentContainer<TComponent>* ecs::EntityBuffer::TryContainerAt() const
{
	const ecs::ComponentId typeId = ToTypeId<TComponent, ecs::ComponentTag>();
	const ecs::IComponentContainer* container = TryContainerAt(typeId);
	return static_cast<const ecs::ComponentContainer<TComponent>*>(container);
}