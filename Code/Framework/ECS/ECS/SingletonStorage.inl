
template<typename TSingleton>
ecs::SingletonContainer<TSingleton>& ecs::SingletonStorage::GetAt()
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	ecs::ISingletonContainer& container = GetAt(typeId);
	return static_cast<ecs::SingletonContainer<TSingleton>&>(container);
}

template<typename TSingleton>
const ecs::SingletonContainer<TSingleton>& ecs::SingletonStorage::GetAt() const
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	const ecs::ISingletonContainer& container = GetAt(typeId);
	return static_cast<const ecs::SingletonContainer<TSingleton>&>(container);
}

template<class TSingleton>
bool ecs::SingletonStorage::WasUpdated() const
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	return m_UpdatedCurr.Contains(typeId);
}

template<class TSingleton>
bool ecs::SingletonStorage::IsRegistered() const
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	return m_Containers.Contains(typeId);
}

template<class TSingleton>
void ecs::SingletonStorage::RegisterSingleton()
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	m_Containers.Set(typeId, new ecs::SingletonContainer<TSingleton>());
}

template<class TSingleton>
auto ecs::SingletonStorage::GetSingleton() -> TSingleton&
{
	ecs::SingletonContainer<TSingleton>& container = GetAt<TSingleton>();
	return container.GetData();
}

template<class TSingleton>
void ecs::SingletonStorage::UpdateSingleton()
{
	const ecs::SingletonId typeId = ToTypeId<TSingleton, ecs::SingletonId>();
	m_UpdatedNext.Add(typeId);
}
