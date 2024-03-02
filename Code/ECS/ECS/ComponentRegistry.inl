#pragma once

template<class TComponent>
void ecs::ComponentRegistry::Register()
{
	static_assert(std::is_convertible<TComponent*, ecs::Component<TComponent>*>::value, "Component must inherit from ecs::Component using the [public] keyword!");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");
	Z_PANIC(!IsRegistered<TComponent>(), "Component has already been registered!");

	using NonConst = std::remove_const<TComponent>::type;
	const ecs::ComponentId componentId = ToTypeIndex<NonConst, ecs::ComponentTag>();

	ecs::ComponentEntry& entry = m_Entries.Emplace(componentId);
	entry.m_Name = ToTypeName<NonConst>();
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