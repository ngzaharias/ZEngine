#pragma once

template<class TResource>
bool ecs::ResourceRegistry::IsRegistered() const
{
	const ecs::ResourceId resourceId = ToTypeIndex<TResource, ecs::ResourceTag>();
	return m_Entries.Contains(resourceId);
}

template<class TResource>
void ecs::ResourceRegistry::Register(TResource& resource)
{
	const ecs::ResourceId resourceId = ToTypeIndex<TResource, ecs::ResourceTag>();
	ecs::ResourceEntry& entry = m_Entries.Emplace(resourceId);
	entry.m_Resource = (void*)&resource;
	entry.m_Name = ToTypeName<TResource>();
}

template<class TResource>
TResource& ecs::ResourceRegistry::Get()
{
	const ecs::ResourceId resourceId = ToTypeIndex<TResource, ecs::ResourceTag>();
	const ecs::ResourceEntry& entry = m_Entries.Get(resourceId);
	return *static_cast<TResource*>(entry.m_Resource);
}