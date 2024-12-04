#pragma once

template<class TComponent>
void ecs::FrameBuffer::RegisterComponent()
{
	using NonConst = std::remove_const<TComponent>::type;
	using Storage = ecs::ComponentStorage<NonConst>;

	const ecs::ComponentId componentId = ToTypeIndex<NonConst, ecs::ComponentTag>();
	m_Components.Set(componentId, new Storage());
}

template<class TComponent, typename... TArgs>
auto ecs::FrameBuffer::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
{
	using NonConst = std::remove_const<TComponent>::type;
	using Storage = ecs::ComponentStorage<NonConst>;

	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Added.Raise(componentId);

	ecs::IComponentStorage* istorage = m_Components.Get(componentId);
	Storage* storage = static_cast<Storage*>(istorage);
	return storage->Emplace(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::FrameBuffer::RemoveComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Removed.Raise(componentId);
}

template<class TComponent>
void ecs::FrameBuffer::UpdateComponent(const ecs::Entity& entity)
{
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	m_EntityChanges[entity].m_Updated.Raise(componentId);
}