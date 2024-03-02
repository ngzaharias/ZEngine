#pragma once

template<class TResource>
void ecs::ResourceRegistry::Register(TResource& resource)
{
	Z_ASSERT_CRASH(!IsRegistered<TResource>(), "Resource has already been registered!");

	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();

	ecs::ResourceEntry& entry = m_Entries.Emplace(resourceId);
	entry.m_Resource = (void*)&resource;
	entry.m_Name = ToTypeName<NonConst>();
}

template<class TResource>
bool ecs::ResourceRegistry::IsRegistered() const
{
	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();
	return m_Entries.Contains(resourceId);
}

template<class TResource>
TResource& ecs::ResourceRegistry::GetResource()
{
	Z_ASSERT_CRASH(IsRegistered<TResource>(), "Resource hasn't been registered!");

	using NonConst = std::remove_const<TResource>::type;
	const ecs::ResourceId resourceId = ToTypeIndex<NonConst, ecs::ResourceTag>();
	const ecs::ResourceEntry& entry = m_Entries.Get(resourceId);
	return *static_cast<NonConst*>(entry.m_Resource);
}