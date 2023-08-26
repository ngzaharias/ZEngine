#pragma once

#include <set>

using int32 = int32_t;

template<typename Type>
class Set
{
public:
	using iterator = typename std::set<Type>::iterator;
	using const_iterator = typename std::set<Type>::const_iterator;
	using value_type = Type;

public:
	Set();
	Set(Set&& rhs);
	Set(const Set& rhs);
	Set(std::initializer_list<Type>&& range);

	auto operator=(Set&& rhs)->Set&;
	auto operator=(const Set& rhs)->Set&;
	auto operator=(std::initializer_list<Type>&& range)->Set&;

	/// \brief Checks if the container is empty.
	bool IsEmpty() const;
	/// \brief Gets the number of elements in the container.
	int32 GetCount() const;

	/// \brief Checks if an element in the container exists.
	bool Contains(const Type& value) const;

	/// \brief Adds a new element to the container.
	void Add(Type&& value);
	/// \brief Adds a new element to the container.
	void Add(const Type& value);
	/// \brief Adds a range of elements to the container.
	void Add(Set&& range);
	/// \brief Adds a range of elements to the container.
	void Add(const Set& range);

	/// \brief Removes an element from the container if it exists.
	void Remove(const Type& value);
	/// \brief Removes a range of elements from the container if they exist.
	void Remove(const Set& range);

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

	auto insert(iterator itr, const Type& value);

private:
	std::set<Type> m_Values = { };
};

#include "Set.inl"