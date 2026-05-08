#pragma once

template<typename TKey, typename TObject>
void eng::TableManager<TKey, TObject>::Load(const str::Path& filepath)
{
	m_ObjectMap.RemoveAll();

	Visitor visitor;
	if (visitor.LoadFromFile(filepath))
		visitor.Read(m_ObjectMap);
}

template<typename TKey, typename TObject>
void eng::TableManager<TKey, TObject>::AddObject(const TKey& key, TObject&& value)
{
	m_ObjectMap.Set(key, std::forward(value));
}


template<typename TKey, typename TObject>
void eng::TableManager<TKey, TObject>::AddObject(const TKey& key, const TObject& value)
{
	m_ObjectMap.Set(key, value);
}

template<typename TKey, typename TObject>
bool eng::TableManager<TKey, TObject>::HasObject(const TKey& key) const
{
	return m_ObjectMap.Contains(key);
}

template<typename TKey, typename TObject>
auto eng::TableManager<TKey, TObject>::GetObject(const TKey& key) const -> const TObject&
{
	return m_ObjectMap.Get(key);
}

template<typename TKey, typename TObject>
auto eng::TableManager<TKey, TObject>::GetObjects() const -> const ObjectMap&
{ 
	return m_ObjectMap; 
}