#pragma once

template<typename TComponent>
void eng::TemplateManager::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::TemplateComponent>, "Type doesn't inherit from ecs::TemplateComponent.");

	const str::Name typeName = NAME(TypeName<TComponent>::m_WithNamespace);
	eng::TemplateEntry& entry = m_EntryMap[typeName];
	entry.m_Read = &ReadComponent<TComponent>;
	entry.m_Write = &WriteComponent<TComponent>;
}

template<typename TComponent>
void eng::TemplateManager::ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor)
{
	static const str::Name typeName = NAME(TypeName<TComponent>::m_WithNamespace);
	if (world.HasComponent<TComponent>(entity))
	{
		const auto& component = world.ReadComponent<TComponent>(entity);
		visitor.Write(typeName, component);
	}
}

template<typename TComponent>
void eng::TemplateManager::WriteComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor)
{
	if (!world.IsRegistered<TComponent>())
		return;

	auto& component = world.HasComponent<TComponent>(entity)
		? world.WriteComponent<TComponent>(entity)
		: world.AddComponent<TComponent>(entity);
	visitor.Read(component);
}