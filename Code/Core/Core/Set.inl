#pragma once

#include "Core/TypeTraits.h"

namespace detail
{
	template<typename Type>
	using HasLessThanOperator = decltype(std::declval<const Type&>() < std::declval<const Type&>());
}

template<typename Type>
Set<Type>::Set()
	: m_Values()
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(core::IsDetected<detail::HasLessThanOperator, Type>::value, "Implement method 'bool operator<(const Type& rhs) const'.");
}

template<typename Type>
Set<Type>::Set(Set&& rhs)
	: m_Values(std::move(rhs.m_Values))
{
}

template<typename Type>
Set<Type>::Set(const Set& rhs)
	: m_Values(rhs.m_Values)
{
}

template<typename Type>
Set<Type>::Set(std::initializer_list<Type>&& range)
	: m_Values(std::move(range))
{
}

template<typename Type>
auto Set<Type>::operator=(Set&& rhs)->Set&
{
	m_Values = std::move(rhs.m_Values);
	return *this;
}

template<typename Type>
auto Set<Type>::operator=(const Set& rhs)->Set&
{
	m_Values = rhs.m_Values;
	return *this;
}

template<typename Type>
auto Set<Type>::operator=(std::initializer_list<Type>&& range)->Set&
{
	m_Values = std::move(range);
	return *this;
}

template<typename Type>
bool Set<Type>::IsEmpty() const
{
	return m_Values.empty();
}

template<typename Type>
int32 Set<Type>::GetCount() const
{
	return static_cast<int32>(m_Values.size());
}

template<typename Type>
bool Set<Type>::Contains(const Type& value) const
{
	return m_Values.count(value) > 0;
}

template<typename Type>
void Set<Type>::Add(Type&& value)
{
	static_assert(std::is_move_constructible<Type>::value, "Implement Move Constructor.");
	m_Values.insert(std::move(value));
}

template<typename Type>
void Set<Type>::Add(const Type& value)
{
	m_Values.insert(value);
}

template<typename Type>
void Set<Type>::Add(Set&& range)
{
	m_Values.insert(range.begin(), range.end());
}

template<typename Type>
void Set<Type>::Add(const Set& range)
{
	m_Values.insert(range.begin(), range.end());
}

template<typename Type>
void Set<Type>::Remove(const Type& value)
{
	m_Values.erase(value);
}

template<typename Type>
void Set<Type>::Remove(const Set<Type>& range)
{
	for (const Type& value : range)
		m_Values.erase(value);
}

template<typename Type>
void Set<Type>::RemoveAll()
{
	m_Values.clear();
}

template<typename Type>
auto Set<Type>::insert(iterator itr, const Type& value)
{
	return m_Values.insert(itr, value);
}