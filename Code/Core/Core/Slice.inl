#pragma once

#include "Core/Assert.h"

template<typename Type>
Slice<Type>::Slice()
	: m_Begin(nullptr)
	, m_End(nullptr)
{
}

template<typename Type>
Slice<Type>::Slice(Type* container, const int32 count)
	: m_Begin(container)
	, m_End(container + count)
{
}

template<typename Type>
Slice<Type>::Slice(Type* begin, Type* end)
	: m_Begin(begin)
	, m_End(end)
{
}

template<typename Type>
Slice<Type>::Slice(const Slice& rhs)
	: m_Begin(rhs.m_Begin)
	, m_End(rhs.m_End)
{
}

template<typename Type>
auto Slice<Type>::operator=(const Slice& rhs)->Slice&
{
	m_Begin = rhs.m_Begin;
	m_End = rhs.m_End;
	return *this;
}

template<typename Type>
auto Slice<Type>::operator [](const int32 index)->Type&
{
	Z_PANIC(index >= 0 && index < GetCount(), "Index out of bounds!");
	return m_Begin[index];
}

template<typename Type>
auto Slice<Type>::operator [](const int32 index) const -> const Type&
{
	Z_PANIC(index >= 0 && index < GetCount(), "Index out of bounds!");
	return m_Begin[index];
}

template<typename Type>
Slice<Type>::operator Slice<const Type>() const
{
	return { m_Begin, m_End };
}

template<typename Type>
bool Slice<Type>::IsEmpty() const
{
	return m_Begin == m_End;
}

template<typename Type>
bool Slice<Type>::IsNull() const
{
	return m_Begin == nullptr;
}

template<typename Type>
int32 Slice<Type>::GetCount() const
{
	const intptr_t count = m_End - m_Begin;
	return static_cast<int32>(count);
}

template<typename Type>
Type* Slice<Type>::GetData()
{
	return m_Begin;
}

template<typename Type>
const Type* Slice<Type>::GetData() const
{
	return m_Begin;
}

template<typename Type>
auto Slice<Type>::GetFirst()->Type&
{
	return *m_Begin;
}

template<typename Type>
auto Slice<Type>::GetFirst() const -> const Type&
{
	return *m_Begin;
}

template<typename Type>
auto Slice<Type>::GetLast()->Type&
{
	return *(m_End - 1);
}

template<typename Type>
auto Slice<Type>::GetLast() const -> const Type&
{
	return *(m_End - 1);
}