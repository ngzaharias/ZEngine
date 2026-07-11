#pragma once

#include "Core/Assert.h"

template<typename Key, typename Value>
bool SparseSet<Key, Value>::Contains(const Key& key) const
{
	static_assert(requires (Key a, Key b) { a == b; }, "Implement method 'bool operator==(const Key& rhs) const'.");

	const int32 sparseIndex = GetIndex(key);
	if (sparseIndex >= m_Sparse.GetCount())
		return false;

	const int32 denseIndex = m_Sparse[sparseIndex];
	if (denseIndex == s_Unassigned)
		return false;
	if (denseIndex >= m_Values.GetCount())
		return false;
	if (m_Dense[denseIndex] != key)
		return false;

	return true;
}

template<typename Key, typename Value>
int32 SparseSet<Key, Value>::GetCapacity() const
{
	return m_Values.GetCapacity();
}

template<typename Key, typename Value>
bool SparseSet<Key, Value>::IsEmpty() const
{
	return m_Values.IsEmpty();
}

template<typename Key, typename Value>
int32 SparseSet<Key, Value>::GetCount() const
{
	return m_Values.GetCount();
}

// #todo: use uint32 so you can't have negative values ?
template<typename Key, typename Value>
int32 SparseSet<Key, Value>::GetIndex(const Key& key) const
{
	if constexpr (requires { key.GetIndex(); })
	{
		return key.GetIndex();
	}
	else if constexpr (std::is_integral<Key>::value)
	{
		return key;
	}
	else
	{
		static_assert(false,
			"Can't use SpSparseSet 'Key' type as an index as it isn't an integral. "
			"Implement method 'int32 GetIndex() const'.");
	}
}

template<typename Key, typename Value>
void SparseSet<Key, Value>::Reserve(const int32 count)
{
	m_Dense.Reserve(count);
	m_Values.Reserve(count);
}

template<typename Key, typename Value>
Value& SparseSet<Key, Value>::Get(const Key& key)
{
	Z_PANIC(Contains(key), "");
	const int32 sparseIndex = GetIndex(key);
	return m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
const Value& SparseSet<Key, Value>::Get(const Key& key) const
{
	Z_PANIC(Contains(key), "");
	const int32 sparseIndex = GetIndex(key);
	return m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
Value* SparseSet<Key, Value>::Try(const Key& key)
{
	if (!Contains(key))
		return nullptr;

	const int32 sparseIndex = GetIndex(key);
	return &m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
const Value* SparseSet<Key, Value>::Try(const Key& key) const
{
	if (!Contains(key))
		return nullptr;

	const int32 sparseIndex = GetIndex(key);
	return &m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
const Array<Key>& SparseSet<Key, Value>::GetKeys() const
{
	return m_Dense;
}

template<typename Key, typename Value>
Array<Value>& SparseSet<Key, Value>::GetValues()
{
	return m_Values;
}

template<typename Key, typename Value>
const Array<Value>& SparseSet<Key, Value>::GetValues() const
{
	return m_Values;
}

template<typename Key, typename Value>
template<typename... Args>
Value& SparseSet<Key, Value>::Emplace(const Key& key, Args&& ...args)
{
	static_assert(std::is_move_constructible<Value>::value, "SparseSet 'Value' type is not move-constructible.");
	static_assert(std::is_move_assignable<Value>::value, "SparseSet 'Value' type is not move-assignable.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 denseIndex = m_Sparse[sparseIndex];
		return m_Values[denseIndex] = std::move(Value{ std::forward<Args>(args)... });
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		if (sparseIndex >= m_Sparse.GetCount())
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 denseIndex = GetCount();
		m_Sparse[sparseIndex] = denseIndex;

		m_Dense.Append(key);
		return m_Values.Append(std::move(Value{ std::forward<Args>(args)... }));
	}
}

template<typename Key, typename Value>
Value& SparseSet<Key, Value>::Set(const Key& key, Value&& value)
{
	static_assert(std::is_move_constructible<Value>::value, "SparseSet 'Value' type is not move-constructible.");
	static_assert(std::is_move_assignable<Value>::value, "SparseSet 'Value' type is not move-assignable.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 denseIndex = m_Sparse[sparseIndex];
		return m_Values[denseIndex] = std::move(value);
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		if (sparseIndex >= m_Sparse.GetCount())
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 denseIndex = GetCount();
		m_Sparse[sparseIndex] = denseIndex;

		m_Dense.Append(key);
		return m_Values.Append(std::move(value));
	}
}

template<typename Key, typename Value>
Value& SparseSet<Key, Value>::Set(const Key& key, const Value& value)
{
	static_assert(std::is_copy_constructible<Value>::value, "SparseSet 'Value' type is not copy-constructible.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 denseIndex = m_Sparse[sparseIndex];
		return m_Values[denseIndex] = std::move(value);
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		if (sparseIndex >= m_Sparse.GetCount())
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 denseIndex = GetCount();
		m_Sparse[sparseIndex] = denseIndex;

		m_Dense.Append(key);
		return m_Values.Append(value);
	}
}

template<typename Key, typename Value>
void SparseSet<Key, Value>::Remove(const Key& key)
{
	static_assert(std::is_move_assignable<Value>::value, "SparseSet 'Value' type is not move-assignable.");
	Z_PANIC(Contains(key), "Trying to remove a value that doesn't exist!");

	const int32 sparseIndex = GetIndex(key);
	const int32 denseIndex = m_Sparse[sparseIndex];
	const int32 denseLast = m_Dense.GetCount() - 1;

	// do the swap
	const int32 sparseSwap = GetIndex(m_Dense[denseLast]);
	std::swap(m_Dense[denseIndex], m_Dense.GetLast());
	std::swap(m_Values[denseIndex], m_Values.GetLast());

	// remove values
	m_Sparse[sparseIndex] = s_Unassigned;
	m_Sparse[sparseSwap] = denseIndex;

	// pop the data
	m_Values.Pop();
	m_Dense.Pop();
}

template<typename Key, typename Value>
void SparseSet<Key, Value>::RemoveAll()
{
	m_Sparse.RemoveAll();
	m_Dense.RemoveAll();
	m_Values.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
SparseSet<Key, Value>::Iterator::Iterator(Dense& dense, Values& values, const int32 index)
	: m_Dense(dense)
	, m_Values(values)
	, m_Index(index)
{
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::Iterator::operator*() -> Pair
{
	return { m_Dense[m_Index], m_Values[m_Index] };
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::Iterator::operator++() -> Iterator&
{
	m_Index++;
	return *this;
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::Iterator::operator++(int32) -> Iterator
{
	Iterator iterator = *this;
	m_Index++;
	return iterator;
}

template<typename Key, typename Value>
bool SparseSet<Key, Value>::Iterator::operator==(const Iterator& rhs) const
{
	return m_Index == rhs.m_Index;
}

template<typename Key, typename Value>
bool SparseSet<Key, Value>::Iterator::operator!=(const Iterator& rhs) const
{
	return m_Index != rhs.m_Index;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
SparseSet<Key, Value>::IteratorConst::IteratorConst(const Dense& dense, const Values& values, const int32 index)
	: m_Dense(dense)
	, m_Values(values)
	, m_Index(index)
{
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::IteratorConst::operator*() -> PairConst
{
	return { m_Dense[m_Index], m_Values[m_Index] };
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::IteratorConst::operator++() -> IteratorConst&
{
	m_Index++;
	return *this;
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::IteratorConst::operator++(int32) -> IteratorConst
{
	IteratorConst iterator = *this;
	m_Index++;
	return iterator;
}

template<typename Key, typename Value>
bool SparseSet<Key, Value>::IteratorConst::operator==(const IteratorConst& rhs) const
{
	return m_Index == rhs.m_Index;
}

template<typename Key, typename Value>
bool SparseSet<Key, Value>::IteratorConst::operator!=(const IteratorConst& rhs) const
{
	return m_Index != rhs.m_Index;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
auto SparseSet<Key, Value>::begin() -> Iterator
{
	return Iterator(m_Dense, m_Values, 0);
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::end() -> Iterator
{
	return Iterator(m_Dense, m_Values, m_Values.GetCount());
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::begin() const -> IteratorConst
{
	return IteratorConst(m_Dense, m_Values, 0);
}

template<typename Key, typename Value>
auto SparseSet<Key, Value>::end() const -> IteratorConst
{
	return IteratorConst(m_Dense, m_Values, m_Values.GetCount());
}
