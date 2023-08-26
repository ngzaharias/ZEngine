#pragma once

#include <map>

using int32 = int32_t;

template<typename Key, typename Value>
class Map
{
public:
	using iterator = typename std::map<Key, Value>::iterator;
	using const_iterator = typename std::map<Key, Value>::const_iterator;
	using key_type = Key;
	using value_type = Value;

public:
	Map();
	Map(Map&& rhs);
	Map(const Map& rhs);
	Map(std::initializer_list<std::pair<const Key, Value>>&& range);

	/// \brief Gets an element from the container if one exists with that key.
	/// Default constructs and inserts the element if it doesn't exist.
	auto operator[](const Key& key)->Value&;

	auto operator=(Map&& rhs)->Map&;
	auto operator=(const Map& rhs)->Map&;
	auto operator=(std::initializer_list<std::pair<const Key, Value>>&& range)->Map&;

	/// \brief Checks if the container is empty.
	bool IsEmpty() const;
	/// \brief Gets the number of elements in the container.
	int32 GetCount() const;

	/// \brief Checks if an element in the container exists with that key.
	bool Contains(const Key& key) const;

	/// \brief Gets an iterator to an element in the container that matches that key.
	/// A past-the-end iterator is returned if none exists.
	auto Find(const Key& key) -> iterator;
	/// \brief Gets an iterator to an element in the container that matches that key.
	/// A past-the-end iterator is returned if none exists.
	auto Find(const Key& key) const ->const_iterator;

	/// \brief Gets an element from the container if one exists with that key.
	/// An exception std::out_of_range is thrown if none exists.
	auto Get(const Key& key)->Value&;
	/// \brief Gets an element from the container if one exists with that key.
	/// An exception std::out_of_range is thrown if none exists.
	auto Get(const Key& key) const -> const Value&;
	/// \brief Gets a copy of an element from the container if one exists with 
	/// that key and if none exists returns the fallback value instead.
	auto Get(const Key& key, const Value& fallback) const -> Value;

	/// \brief Inserts a new element that is constructed in-place if 
	/// the container doesn't already contain an element with that key.
	template<typename... Args>
	void Emplace(const Key& key, Args&&... args);

	/// \brief Inserts a new element if the container doesn't already contain an element with that key.
	void Insert(const Key& key, Value&& value);
	/// \brief Inserts a new element if the container doesn't already contain an element with that key.
	void Insert(const Key& key, const Value& value);

	/// \brief Inserts a new element if the container doesn't already contain an 
	/// element with that key, otherwise it will replace the existing element.
	void Set(const Key& key, Value&& value);
	/// \brief Inserts a new element if the container doesn't already contain an 
	/// element with that key, otherwise it will replace the existing element.
	void Set(const Key& key, const Value& value);

	/// \brief Removes a specified element from the container if one exists with that key.
	void Remove(const Key& key);

	/// \brief Removes all elements from the container.
	void RemoveAll();

public:
	auto begin() { return m_Values.begin(); }
	auto rbegin() { return m_Values.rbegin(); }

	auto end() { return m_Values.end(); }
	auto rend() { return m_Values.rend(); }

	auto begin() const { return m_Values.begin(); }
	auto cbegin() const { return m_Values.cbegin(); }
	auto rbegin() const { return m_Values.rbegin(); }
	auto crbegin() const { return m_Values.crbegin(); }

	auto end() const { return m_Values.end(); }
	auto cend() const { return m_Values.cend(); }
	auto rend() const { return m_Values.rend(); }
	auto crend() const { return m_Values.crend(); }

private:
	std::map<Key, Value> m_Values = { };
};

#include "Map.inl"