#pragma once

template<typename TKey, typename TValue>
void eng::TableManager<TKey, TValue>::Read(Visitor& visitor)
{
	m_ObjectMap.RemoveAll();
	visitor.Read(m_ObjectMap);
}