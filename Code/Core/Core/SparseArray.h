#pragma once

#include "Core/Array.h"
#include "Core/Set.h"
#include "Core/Tuple.h"

// #todo: support indexes other than int32

using int32 = int32_t;

template<typename Key, typename Value>
class SparseArray final
{
	using Dense = Array<Key>;
	using Sparse = Array<int32>;
	using Values = Array<Value>;

public:
	static constexpr int32 s_Unassigned = -1;

	/// \brief Returns true if the key is found in the sparse.
	bool Contains(const Key& key) const;

	/// \brief Checks if the container is empty.
	bool IsEmpty() const;

	/// \brief Gets the number of elements the values array is sized for.
	int32 GetCapacity() const;
	/// \brief Gets the number of elements in the values array.
	int32 GetCount() const;
	/// \brief Gets the index from the key.
	int32 GetIndex(const Key& key) const;

	/// \brief Reserves the dense and values array to at least X size without appending elements.
	void Reserve(const int32 count);

	/// \brief Gets an element from the container if one exists with that key.
	/// An exception std::out_of_range is thrown if none exists.
	Value& Get(const Key& key);
	/// \brief Gets an element from the container if one exists with that key.
	/// An exception std::out_of_range is thrown if none exists.
	const Value& Get(const Key& key) const;

	/// \brief Returns the dense array.
	const Array<Key>& GetKeys() const;

	/// \brief Returns the values array.
	Array<Value>& GetValues();
	/// \brief Returns the values array.
	const Array<Value>& GetValues() const;

	/// \brief Sets a new element in the container that is constructed in-place.
	template<typename... Args>
	Value& Emplace(const Key& key, Args&& ...args);

	/// \brief Sets a new element in the container using move operator.
	Value& Set(const Key& key, Value&& value);
	/// \brief Sets a new element in the container.
	Value& Set(const Key& key, const Value& value);

	/// \brief Removes a specified element from the container if one exists with that key.
	void Remove(const Key& key);

	/// \brief Removes all elements from the container.
	void RemoveAll();

public:
	struct Pair
	{
		const Key& m_Key;
		Value& m_Value;
	};

	struct PairConst
	{
		const Key& m_Key;
		const Value& m_Value;
	};

	struct Iterator
	{
		Iterator(Dense& dense, Values& values, const int32 index);

		auto operator*()->Pair;
		auto operator++()->Iterator&;
		auto operator++(int32)->Iterator;
		bool operator==(const Iterator& rhs) const;
		bool operator!=(const Iterator& rhs) const;

	private:
		int32 m_Index;
		Dense& m_Dense;
		Values& m_Values;
	};

	struct IteratorConst
	{
		IteratorConst(const Dense& dense, const Values& values, const int32 index);

		auto operator*()->PairConst;
		auto operator++()->IteratorConst&;
		auto operator++(int32)->IteratorConst;
		bool operator==(const IteratorConst& rhs) const;
		bool operator!=(const IteratorConst& rhs) const;

	private:
		int32 m_Index;
		const Dense& m_Dense;
		const Values& m_Values;
	};

	auto begin() -> Iterator;
	auto end() -> Iterator;
	auto begin() const -> IteratorConst;
	auto end() const -> IteratorConst;

private:
	/// \brief Maps an index in the m_Values array to its Key.
	/// Used for quick removal of individual items and can be used for iterating over all keys.
	Dense m_Dense = { };

	/// \brief Maps a Key to its index in the m_Values array.
	Sparse m_Sparse = { };

	/// \brief Holds the actual data/values.
	Values m_Values = { };
};

#include "SparseArray.inl"