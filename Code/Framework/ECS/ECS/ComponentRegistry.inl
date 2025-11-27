#pragma once

template<class TComponent>
void ecs::ComponentRegistry::Register()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(std::is_convertible<TComponent*, ecs::Component<TComponent>*>::value, "Component must inherit from ecs::Component using the [public] keyword!");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(!IsRegistered<TComponent>(), "Component is already registered!");

	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	ecs::ComponentEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TComponent>();
}

template<class TComponent>
bool ecs::ComponentRegistry::IsRegistered() const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");

	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Contains(id);
}

template<class TComponent>
auto ecs::ComponentRegistry::GetEntry() const -> const ecs::ComponentEntry&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Get(id);
}