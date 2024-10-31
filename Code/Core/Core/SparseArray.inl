#pragma once

#include "Core/Assert.h"
#include "Core/TypeTraits.h"

namespace detail
{
	template<typename Key>
	using HasGetIndexMethod = decltype(std::declval<const Key&>().GetIndex());

	template<typename Key>
	using HasIsEqualOperator = decltype(std::declval<const Key&>() == std::declval<const Key&>());
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::Contains(const Key& key) const
{
	static_assert(core::IsDetected<detail::HasIsEqualOperator, Key>::value, "Implement method 'bool operator==(const Key& rhs) const'.");

	const int32 sparseIndex = GetIndex(key);
	if (sparseIndex < 0 || sparseIndex >= m_Sparse.GetCount())
		return false;

	const int32 valueIndex = m_Sparse[sparseIndex];
	if (valueIndex == s_Unassigned)
		return false;

	return true;
}

template<typename Key, typename Value>
int32 SparseArray<Key, Value>::GetCapacity() const
{
	return m_Values.GetCapacity();
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::IsEmpty() const
{
	return m_Values.IsEmpty();
}

template<typename Key, typename Value>
int32 SparseArray<Key, Value>::GetCount() const
{
	return m_Values.GetCount();
}

// #todo: use uint32 so you can't have negative values ?
template<typename Key, typename Value>
int32 SparseArray<Key, Value>::GetIndex(const Key& key) const
{
	constexpr bool hasCustom = core::IsDetected<detail::HasGetIndexMethod, Key>::value;
	constexpr bool hasIntegral = std::is_integral<Key>::value;
	if constexpr (hasCustom)
	{
		return key.GetIndex();
	}
	else if constexpr (hasIntegral)
	{
		return key;
	}

	static_assert(hasCustom || hasIntegral,
		"Can't use SparseArray's 'Key' type as an index as it isn't an integral. "
		"Implement method 'int32 GetIndex() const'.");
}

template<typename Key, typename Value>
void SparseArray<Key, Value>::Reserve(const int32 count)
{
	m_Dense.Reserve(count);
	m_Values.Reserve(count);
}

template<typename Key, typename Value>
Value& SparseArray<Key, Value>::Get(const Key& key)
{
	const int32 sparseIndex = GetIndex(key);
	return m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
const Value& SparseArray<Key, Value>::Get(const Key& key) const
{
	const int32 sparseIndex = GetIndex(key);
	return m_Values[m_Sparse[sparseIndex]];
}

template<typename Key, typename Value>
const Array<Key>& SparseArray<Key, Value>::GetKeys() const
{
	return m_Dense;
}

template<typename Key, typename Value>
Array<Value>& SparseArray<Key, Value>::GetValues()
{
	return m_Values;
}

template<typename Key, typename Value>
const Array<Value>& SparseArray<Key, Value>::GetValues() const
{
	return m_Values;
}

template<typename Key, typename Value>
template<typename... Args>
Value& SparseArray<Key, Value>::Emplace(const Key& key, Args&& ...args)
{
	static_assert(std::is_move_constructible<Value>::value, "SparseArray 'Value' type is not move-constructible.");
	static_assert(std::is_move_assignable<Value>::value, "SparseArray 'Value' type is not move-assignable.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 valueIndex = m_Sparse[sparseIndex];
		m_Dense[valueIndex] = key;
		return m_Values[valueIndex] = std::move(Value{ std::forward<Args>(args)... });
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		Z_PANIC(sparseIndex >= 0, "Keys cannot have an index with a negative value!");

		const int32 sparseCount = m_Sparse.GetCount();
		if (sparseIndex >= sparseCount)
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 valueIndex = GetCount();
		m_Dense.Append(key);
		m_Sparse[sparseIndex] = valueIndex;
		m_Values.Append(std::move(Value{ std::forward<Args>(args)... }));
		return m_Values[valueIndex];
	}
}

template<typename Key, typename Value>
Value& SparseArray<Key, Value>::Set(const Key& key, Value&& value)
{
	static_assert(std::is_copy_constructible<Value>::value, "SparseArray 'Value' type is not copy-constructible.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 valueIndex = m_Sparse[sparseIndex];
		m_Dense[valueIndex] = key;
		return m_Values[valueIndex] = std::move(value);
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		Z_PANIC(sparseIndex >= 0, "Keys cannot have an index with a negative value!");

		const int32 sparseCount = m_Sparse.GetCount();
		if (sparseIndex >= sparseCount)
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 valueIndex = GetCount();
		m_Dense.Append(key);
		m_Sparse[sparseIndex] = valueIndex;
		m_Values.Append(std::move(value));
		return m_Values[valueIndex];
	}
}

template<typename Key, typename Value>
Value& SparseArray<Key, Value>::Set(const Key& key, const Value& value)
{
	static_assert(std::is_copy_constructible<Value>::value, "SparseArray 'Value' type is not copy-constructible.");

	if (Contains(key))
	{
		const int32 sparseIndex = GetIndex(key);
		const int32 valueIndex = m_Sparse[sparseIndex];
		m_Dense[valueIndex] = key;
		return m_Values[valueIndex] = value;
	}
	else
	{
		const int32 sparseIndex = GetIndex(key);
		Z_PANIC(sparseIndex >= 0, "Keys cannot have an index with a negative value!");

		const int32 sparseCount = m_Sparse.GetCount();
		if (sparseIndex >= sparseCount)
			m_Sparse.Resize(sparseIndex + 1, s_Unassigned);

		const int32 valueIndex = GetCount();
		m_Dense.Append(key);
		m_Sparse[sparseIndex] = valueIndex;
		m_Values.Append(value);
		return m_Values[valueIndex];
	}
}

template<typename Key, typename Value>
void SparseArray<Key, Value>::Remove(const Key& key)
{
	static_assert(std::is_move_assignable<Value>::value, "SparseArray 'Value' type is not move-assignable.");

	Z_PANIC(Contains(key), "");

	const int32 sparseIndex = GetIndex(key);
	const int32 valueIndex = m_Sparse[sparseIndex];
	const int32 valueLast = GetCount() - 1;

	if (valueIndex == valueLast)
	{
		m_Sparse[sparseIndex] = s_Unassigned;
		m_Values.Pop();
		m_Dense.Pop();
	}
	else
	{
		const int32 sparseSwap = GetIndex(m_Dense[valueLast]);

		// do the swap
		std::swap(m_Dense[valueIndex], m_Dense[valueLast]);
		std::swap(m_Values[valueIndex], m_Values[valueLast]);

		// remove values
		m_Sparse[sparseIndex] = s_Unassigned;
		m_Sparse[sparseSwap] = valueIndex;

		// pop the data
		m_Values.Pop();
		m_Dense.Pop();
	}
}

template<typename Key, typename Value>
void SparseArray<Key, Value>::RemoveAll()
{
	m_Dense.RemoveAll();
	m_Sparse.RemoveAll();
	m_Values.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
SparseArray<Key, Value>::Iterator::Iterator(Dense& dense, Values& values, const int32 index)
	: m_Dense(dense)
	, m_Values(values)
	, m_Index(index)
{
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::Iterator::operator*() -> Pair
{
	return { m_Dense[m_Index], m_Values[m_Index] };
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::Iterator::operator++() -> Iterator&
{
	m_Index++;
	return *this;
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::Iterator::operator++(int32) -> Iterator
{
	Iterator iterator = *this;
	m_Index++;
	return iterator;
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::Iterator::operator==(const Iterator& rhs) const
{
	return m_Index == rhs.m_Index;
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::Iterator::operator!=(const Iterator& rhs) const
{
	return m_Index != rhs.m_Index;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
SparseArray<Key, Value>::IteratorConst::IteratorConst(const Dense& dense, const Values& values, const int32 index)
	: m_Dense(dense)
	, m_Values(values)
	, m_Index(index)
{
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::IteratorConst::operator*() -> PairConst
{
	return { m_Dense[m_Index], m_Values[m_Index] };
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::IteratorConst::operator++() -> IteratorConst&
{
	m_Index++;
	return *this;
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::IteratorConst::operator++(int32) -> IteratorConst
{
	IteratorConst iterator = *this;
	m_Index++;
	return iterator;
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::IteratorConst::operator==(const IteratorConst& rhs) const
{
	return m_Index == rhs.m_Index;
}

template<typename Key, typename Value>
bool SparseArray<Key, Value>::IteratorConst::operator!=(const IteratorConst& rhs) const
{
	return m_Index != rhs.m_Index;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
auto SparseArray<Key, Value>::begin() -> Iterator
{
	return Iterator(m_Dense, m_Values, 0);
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::end() -> Iterator
{
	return Iterator(m_Dense, m_Values, m_Values.GetCount());
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::begin() const -> IteratorConst
{
	return IteratorConst(m_Dense, m_Values, 0);
}

template<typename Key, typename Value>
auto SparseArray<Key, Value>::end() const -> IteratorConst
{
	return IteratorConst(m_Dense, m_Values, m_Values.GetCount());
}
