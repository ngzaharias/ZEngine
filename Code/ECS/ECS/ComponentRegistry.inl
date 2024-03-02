#pragma once

template<class TComponent>
void ecs::ComponentRegistry::Register()
{
	static_assert(std::is_convertible<TComponent*, ecs::Component<TComponent>*>::value, "Component must inherit from ecs::Component using the [public] keyword!");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");
	Z_PANIC(!IsRegistered<TComponent>(), "Component has already been registered!");

	ecs::ComponentEntry entry;
	entry.m_ComponentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	entry.m_DebugName = ToTypeName<TComponent>();

	m_Entries.Set(entry.m_ComponentId, std::move(entry));
}

template<class TComponent>
bool ecs::ComponentRegistry::IsRegistered() const
{
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Contains(componentId);
}

template<class TComponent>
auto ecs::ComponentRegistry::GetEntry() const -> const ecs::ComponentEntry&
{
	Z_PANIC(IsRegistered<TComponent>(), "Component hasn't been registered!");
	const ecs::ComponentId componentId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Get(componentId);
}