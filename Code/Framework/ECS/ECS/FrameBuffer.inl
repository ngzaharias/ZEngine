#pragma once

// No need to do component verification since all that 
// should have been handled by the EntityWorld

template<class TComponent>
void ecs::FrameBuffer::RegisterComponent()
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_Components.Set(id, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent, typename... TArgs>
auto ecs::FrameBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
{
	using Storage = ecs::ComponentStorage<TComponent>;

	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Added.Raise(id);

	ecs::IComponentStorage* istorage = m_Components.Get(id);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::FrameBuffer::RemoveComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Removed.Raise(id);
}

template<class TComponent>
void ecs::FrameBuffer::UpdateComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Updated.Raise(id);
}

template<class TEvent>
void ecs::FrameBuffer::RegisterEvent()
{
	const ecs::EventId id = ToTypeIndex<TEvent, ecs::EventTag>();
	m_Events.Set(id, new ecs::EventStorage<TEvent>());
}

template<class TEvent, typename... TArgs>
auto ecs::FrameBuffer::AddEvent(TArgs&&... args)->decltype(auto)
{
	using Storage = ecs::EventStorage<TEvent>;

	const ecs::EventId id = ToTypeIndex<TEvent, ecs::EventTag>();
	ecs::IEventStorage* istorage = m_Events.Get(id);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(std::forward<TArgs>(args)...);
}

template<class TSingleton>
void ecs::FrameBuffer::UpdateSingleton()
{
	const ecs::EventId id = ToTypeIndex<TSingleton, ecs::SingletonTag>();
	m_Singletons.Add(id);
}