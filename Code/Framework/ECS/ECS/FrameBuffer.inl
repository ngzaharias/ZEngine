#pragma once

// No need to do component verification since all that 
// should have been handled by the EntityWorld

template<class TComponent>
void ecs::FrameBuffer::RegisterComponent()
{
	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	m_Components.Set(componentId, new ecs::ComponentStorage<TComponent>());
}

template<class TComponent, typename... TArgs>
auto ecs::FrameBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
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

template<class TEvent>
void ecs::FrameBuffer::RegisterEvent()
{
	const TypeId typeId = ToTypeId<TEvent>();
	m_Events.Set(typeId, new ecs::EventStorage<TEvent>());
}

template<class TEvent, typename... TArgs>
auto ecs::FrameBuffer::AddEvent(TArgs&&... args)->decltype(auto)
{
	using Storage = ecs::EventStorage<TEvent>;

	const TypeId typeId = ToTypeId<TEvent>();
	ecs::IEventStorage* istorage = m_Events.Get(typeId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(std::forward<TArgs>(args)...);
}

template<class TSingleton>
void ecs::FrameBuffer::UpdateSingleton()
{
	const TypeId typeId = ToTypeId<TSingleton>();
	m_Singletons.Add(typeId);
}