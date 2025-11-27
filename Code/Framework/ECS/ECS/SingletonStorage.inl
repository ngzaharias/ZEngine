#pragma once

template<typename TSingleton>
inline TSingleton& ecs::SingletonStorage<TSingleton>::GetData()
{
	return m_Data;
}

template<typename TSingleton>
inline const TSingleton& ecs::SingletonStorage<TSingleton>::GetData() const
{
	return m_Data;
}