
template<typename TSingleton>
inline TSingleton& ecs::SingletonContainer<TSingleton>::GetData()
{
	return m_Data;
}

template<typename TSingleton>
inline const TSingleton& ecs::SingletonContainer<TSingleton>::GetData() const
{
	return m_Data;
}