
template<class TComponent>
void ecs::FrameBuffer::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_Components.Set(componentId, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent, typename... TArgs>
auto ecs::FrameBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Added.Raise(componentId);

	ecs::IComponentStorage* istorage = m_Components.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::FrameBuffer::RemoveComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Removed.Raise(componentId);
}

template<class TComponent>
void ecs::FrameBuffer::UpdateComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Updated.Raise(componentId);
}