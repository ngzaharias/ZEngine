#pragma once

template<class TResource>
bool ecs::ResourceRegistry::Has() const
{
	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();
	return m_Entries.Contains(resourceId);
}

template<class TResource>
TResource& ecs::ResourceRegistry::Get()
{
	Z_PANIC(Has<TResource>(), "Resource hasn't been added!");

	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();
	const ecs::ResourceEntry& entry = m_Entries.Get(resourceId);
	return *static_cast<NonConst*>(entry.m_Resource);
}

template<class TResource>
void ecs::ResourceRegistry::Add(TResource& resource)
{
	Z_PANIC(!Has<TResource>(), "Resource has already been added!");

	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();

	ecs::ResourceEntry& entry = m_Entries.Emplace(resourceId);
	entry.m_Resource = (void*)&resource;
	entry.m_Name = ToTypeName<NonConst>();
}

template<class TResource>
void ecs::ResourceRegistry::Remove(TResource& resource)
{
	Z_PANIC(Has<TResource>(), "Resource hasn't been added!");

	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();

	m_Entries.Remove(resourceId);
}