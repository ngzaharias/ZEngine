#pragma once

template<typename TComponent>
void ecs::ConstructComponent(void* data)
{
	new (data) TComponent();
}

template<typename TComponent>
void ecs::CopystructComponent(void* source, void* target)
{
	new (target) TComponent(*static_cast<TComponent*>(source));
}

template<typename TComponent>
void ecs::DestructComponent(void* data)
{
	TComponent* component = static_cast<TComponent*>(data);
	component->~TComponent();
}

template<typename TComponent>
bool ecs::HasComponent(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	return storage.HasComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity)
{
	buffer.AddComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, const MemBuffer& data)
{
	auto& component = buffer.AddComponent<TComponent>(entity);
	data.Read(component);
}

template<typename TComponent>
void ecs::RemoveComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity)
{
	buffer.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::ReadComponent(ecs::EntityStorage& storage, const ecs::Entity& entity, MemBuffer& data)
{
	const auto& component = storage.GetComponent<TComponent>(entity);
	data.Write(component);
}

template<typename TComponent>
void ecs::WriteComponent(ecs::EntityBuffer& buffer, ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	buffer.UpdateComponent<TComponent>(entity);
	auto& component = storage.GetComponent<TComponent>(entity);
	data.Read(component);
}