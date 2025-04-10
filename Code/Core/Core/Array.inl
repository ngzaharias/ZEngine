#pragma once

#include "Core/Assert.h"

#include <algorithm>

template<typename Type>
Array<Type>::Array()
	: m_Values()
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");

	// #hack: std::vector<bool> behaves differently to other types and a lot of functionality is broken if we allow it.
	// Eventually we will convert to not using std::vector and at that point it can be allowed again.
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
Array<Type>::Array(int32 count)
	: m_Values(count)
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
Array<Type>::Array(int32 count, const Type& value)
	: m_Values(count, value)
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
Array<Type>::Array(Array&& rhs)
	: m_Values(std::move(rhs.m_Values))
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
Array<Type>::Array(const Array& rhs)
	: m_Values(rhs.m_Values)
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
Array<Type>::Array(std::initializer_list<Type>&& range)
	: m_Values(std::move(range))
{
	static_assert(std::is_default_constructible<Type>::value, "Implement Default Constructor.");
	static_assert(!std::is_same<Type, bool>::value, "Restricted Type!");
}

template<typename Type>
auto Array<Type>::operator=(Array&& rhs)->Array&
{
	m_Values = std::move(rhs.m_Values);
	return *this;
}

template<typename Type>
auto Array<Type>::operator=(const Array& rhs)->Array&
{
	m_Values = rhs.m_Values;
	return *this;
}

template<typename Type>
auto Array<Type>::operator=(std::initializer_list<Type>&& rhs)->Array&
{
	m_Values = std::move(rhs);
	return *this;
}

template<typename Type>
auto Array<Type>::operator [](const int32 index)->Type&
{
	Z_PANIC(index >= 0 && index < GetCount(), "Index out of bounds!");
	return m_Values[index];
}

template<typename Type>
auto Array<Type>::operator [](const int32 index) const -> const Type&
{
	Z_PANIC(index >= 0 && index < GetCount(), "Index out of bounds!");
	return m_Values[index];
}

template<typename Type>
Array<Type>::operator Slice<Type>()
{
	return Slice<Type>(GetData(), GetCount());
}

template<typename Type>
Array<Type>::operator Slice<const Type>() const
{
	return Slice<const Type>(GetData(), GetCount());
}

template<typename Type>
bool Array<Type>::IsEmpty() const
{
	return m_Values.empty();
}

template<typename Type>
int32 Array<Type>::GetCapacity() const
{
	return static_cast<int32>(m_Values.capacity());
}

template<typename Type>
int32 Array<Type>::GetCount() const
{
	return static_cast<int32>(m_Values.size());
}

template<typename Type>
Type* Array<Type>::GetData()
{
	return m_Values.data();
}

template<typename Type>
const Type* Array<Type>::GetData() const
{
	return m_Values.data();
}

template<typename Type>
void Array<Type>::Resize(const int32 newSize)
{
	m_Values.resize(newSize);
}

template<typename Type>
void Array<Type>::Resize(const int32 newSize, const Type& value)
{
	m_Values.resize(newSize, value);
}

template<typename Type>
void Array<Type>::Reserve(const int32 count)
{
	m_Values.reserve(count);
}

template<typename Type>
auto Array<Type>::GetFirst()->Type&
{
	return m_Values[0];
}

template<typename Type>
auto Array<Type>::GetFirst() const -> const Type&
{
	return m_Values[0];
}

template<typename Type>
auto Array<Type>::GetLast()->Type&
{
	return m_Values.back();
}

template<typename Type>
auto Array<Type>::GetLast() const -> const Type&
{
	return m_Values.back();
}

template<typename Type>
auto Array<Type>::Pop()->Type
{
	Type value = GetLast();
	RemoveLast();
	return value;
}

template<typename Type>
auto Array<Type>::Append(Type&& value)->Type&
{
	static_assert(std::is_move_constructible<Type>::value, "Implement Move Constructor.");
	m_Values.push_back(std::move(value));
	return GetLast();
}

template<typename Type>
auto Array<Type>::Append(const Type& value)->Type&
{
	m_Values.push_back(value);
	return GetLast();
}

template<typename Type>
void Array<Type>::Append(Array&& range)
{
	std::move(std::begin(range.m_Values), std::end(range.m_Values), std::back_inserter(m_Values));
}

template<typename Type>
void Array<Type>::Append(const Array& range)
{
	m_Values.insert(m_Values.end(), range.begin(), range.end());
}

template<typename Type>
void Array<Type>::Append(std::initializer_list<Type>&& range)
{
	m_Values.insert(m_Values.end(), range);
}

template<typename Type>
template<typename... Args>
auto Array<Type>::Emplace(Args&&... args)->Type&
{
	m_Values.emplace_back(args...);
	return GetLast();
}

template<typename Type>
auto Array<Type>::Insert(Type&& value, const int32 index)->Type&
{
	static_assert(std::is_move_constructible<Type>::value, "Implement Move Constructor.");

	const auto itr = m_Values.begin() + index;
	return *m_Values.insert(itr, std::move(value));
}

template<typename Type>
auto Array<Type>::Insert(const Type& value, const int32 index)->Type&
{
	const auto itr = m_Values.begin() + index;
	return *m_Values.insert(itr, std::move(value));
}

template<typename Type>
void Array<Type>::RemoveAt(const int32 index)
{
	if (m_Values.size() != 1)
		std::swap(m_Values[index], m_Values.back());
	m_Values.pop_back();
}

template<typename Type>
void Array<Type>::RemoveAt(const int32 startIndex, const int32 endIndex)
{
	m_Values.erase(m_Values.begin() + startIndex, m_Values.begin() + endIndex);
}

template<typename Type>
void Array<Type>::RemoveAt(const iterator itr)
{
	if (m_Values.size() != 1)
		std::iter_swap(itr, m_Values.end() - 1);
	m_Values.pop_back();
}

template<typename Type>
void Array<Type>::RemoveOrderedAt(const int32 index)
{
	m_Values.erase(m_Values.begin() + index);
}

template<typename Type>
void Array<Type>::RemoveOrderedAt(const int32 startIndex, const int32 endIndex)
{
	m_Values.erase(m_Values.begin() + startIndex, m_Values.begin() + endIndex);
}

template<typename Type>
void Array<Type>::RemoveOrderedAt(const iterator itr)
{
	m_Values.erase(itr);
}

template<typename Type>
void Array<Type>::RemoveLast()
{
	m_Values.pop_back();
}

template<typename Type>
void Array<Type>::RemoveLast(int32 count)
{
	m_Values.resize(m_Values.size() - count);
}

template<typename Type>
void Array<Type>::RemoveAll()
{
	m_Values.clear();
}

template<typename Type>
auto Array<Type>::insert(iterator itr, const Type& value)
{
	return m_Values.insert(itr, value);
}