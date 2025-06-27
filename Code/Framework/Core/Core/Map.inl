#pragma once

template<typename Key, typename Value>
Map<Key, Value>::Map()
	: m_Values()
{
}

template<typename Key, typename Value>
Map<Key, Value>::Map(Map&& rhs)
	: m_Values(std::move(rhs.m_Values))
{
}

template<typename Key, typename Value>
Map<Key, Value>::Map(const Map& rhs)
	: m_Values(rhs.m_Values)
{
}

template<typename Key, typename Value>
Map<Key, Value>::Map(std::initializer_list<std::pair<const Key, Value>>&& range)
	: m_Values(std::move(range))
{
}

template<typename Key, typename Value>
auto Map<Key, Value>::operator[](const Key& key)->Value&
{
	return m_Values[key];
}

template<typename Key, typename Value>
auto Map<Key, Value>::operator=(Map&& rhs)->Map&
{
	m_Values = std::move(rhs.m_Values);
	return *this;
}

template<typename Key, typename Value>
auto Map<Key, Value>::operator=(const Map& rhs)->Map&
{
	m_Values = rhs.m_Values;
	return *this;
}

template<typename Key, typename Value>
auto Map<Key, Value>::operator=(std::initializer_list<std::pair<const Key, Value>>&& range)->Map&
{
	m_Values = std::move(range);
	return *this;
}

template<typename Key, typename Value>
bool Map<Key, Value>::IsEmpty() const
{
	return m_Values.empty();
}

template<typename Key, typename Value>
int32 Map<Key, Value>::GetCount() const
{
	return static_cast<int32>(m_Values.size());
}

template<typename Key, typename Value>
bool Map<Key, Value>::Contains(const Key& key) const
{
	return m_Values.count(key) > 0;
}

template<typename Key, typename Value>
auto Map<Key, Value>::Find(const Key& key)->iterator
{
	return m_Values.find(key);
}

template<typename Key, typename Value>
auto Map<Key, Value>::Find(const Key& key) const->const_iterator
{
	return m_Values.find(key);
}

template<typename Key, typename Value>
auto Map<Key, Value>::Get(const Key& key) -> Value&
{
	return m_Values.at(key);
}

template<typename Key, typename Value>
auto Map<Key, Value>::Get(const Key& key) const -> const Value&
{
	return m_Values.at(key);
}

template<typename Key, typename Value>
auto Map<Key, Value>::Get(const Key& key, const Value& fallback) const->Value
{
	const auto find = m_Values.find(key);
	if (find != m_Values.end())
		return find->second;
	return fallback;
}

template<typename Key, typename Value>
template<typename... Args>
void Map<Key, Value>::Emplace(const Key& key, Args&&... args)
{
	m_Values.emplace(key, std::forward<Args>(args)...);
}

template<typename Key, typename Value>
void Map<Key, Value>::Insert(const Key& key, Value&& value)
{
	static_assert(std::is_move_constructible<Value>::value, "Implement Move Constructor.");
	m_Values.insert({ key, std::move(value) });
}

template<typename Key, typename Value>
void Map<Key, Value>::Insert(const Key& key, const Value& value)
{
	m_Values.insert({ key, value });
}

template<typename Key, typename Value>
void Map<Key, Value>::Set(const Key& key, Value&& value)
{
	static_assert(std::is_move_constructible<Value>::value, "Implement Move Constructor.");
	m_Values[key] = std::move(value);
}

template<typename Key, typename Value>
void Map<Key, Value>::Set(const Key& key, const Value& value)
{
	m_Values[key] = value;
}

template<typename Key, typename Value>
void Map<Key, Value>::Remove(const Key& key)
{
	m_Values.erase(key);
}

template<typename Key, typename Value>
void Map<Key, Value>::RemoveAll()
{
	m_Values.clear();
}