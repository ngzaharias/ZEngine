#pragma once

#include "Core/Slice.h"

#include <vector>

// https://stackoverflow.com/questions/7758580/writing-your-own-stl-container

using int32 = int32_t;

template<typename Type>
class Array
{
public:
	using iterator = typename std::vector<Type>::iterator;
	using const_iterator = typename std::vector<Type>::iterator;
	using value_type = Type;

public:
	Array();
	Array(int32 count);
	Array(int32 count, const Type& value);
	Array(Array&& rhs);
	Array(const Array& rhs);
	Array(std::initializer_list<Type>&& range);

	auto operator=(Array&& rhs)->Array&;
	auto operator=(const Array& rhs)->Array&;
	auto operator=(std::initializer_list<Type>&& rhs)->Array&;

	auto operator[](const int32 index)->Type&;
	auto operator[](const int32 index) const -> const Type&;

	/// \brief Implicit conversion to Slice.
	operator Slice<Type>();
	/// \brief Implicit conversion to Slice.
	operator Slice<const Type>() const;

	/// \brief Checks if the container is empty.
	bool IsEmpty() const;
	/// \brief Gets the number of elements the container is sized for.
	int32 GetCapacity() const;
	/// \brief Gets the number of elements in the container.
	int32 GetCount() const;

	/// \brief Get a raw pointer to the data container.
	Type* GetData();
	/// \brief Get a raw pointer to the data container.
	const Type* GetData() const;

	/// \brief Resizes the container to X size.
	/// If the current size is greater than, then elements are removed.
	/// If the current size is less than, then elements are default appended.
	void Resize(const int32 newSize);
	/// \brief Resizes the container to X size.
	/// If the current size is greater than, then elements are removed.
	/// If the current size is less than, then elements are appended using the specified value.
	void Resize(const int32 newSize, const Type& value);

	/// \brief Reserves the container to at least X size without appending elements.
	void Reserve(const int32 newCapacity);

	/// \brief Gets the first element from the container.
	auto GetFirst()->Type&;
	/// \brief Gets the first element from the container.
	auto GetFirst() const -> const Type&;

	/// \brief Gets the last element from the container.
	auto GetLast()->Type&;
	/// \brief Gets the last element from the container.
	auto GetLast() const -> const Type&;

	/// \brief Appends a new element to the back of the container.
	auto Append(Type&& value)->Type&;
	/// \brief Appends a new element to the back of the container.
	auto Append(const Type& value)->Type&;
	/// \brief Appends a range of elements to the back of the container.
	void Append(Array&& range);
	/// \brief Appends a range of elements to the back of the container.
	void Append(const Array& range);
	/// \brief Appends a range of elements to the back of the container.
	void Append(std::initializer_list<Type>&& range);

	/// \brief Appends a new element to the back of the container that is constructed in-place.
	template<typename... Args>
	auto Emplace(Args&&... args)->Type&;

	/// \brief Inserts a new element at a specific index in the container.
	/// Assumes that the index is within the containers current bounds.
	auto Insert(Type&& value, const int32 index)->Type&;
	/// \brief Inserts a new element at a specific index in the container.
	/// Assumes that the index is within the containers current bounds.
	auto Insert(const Type& value, const int32 index)->Type&;

	/// \brief Removes the last element from the container and returns a copy of it.
	auto Pop()->Type;


	/// \brief Removes the X element from the container, but doesn't preserve the order of the elements.
	void RemoveAt(const int32 index);
	/// \brief Removes the X until Y elements from the container, but doesn't preserve the order of the elements.
	void RemoveAt(const int32 startIndex, const int32 endIndex);
	/// \brief Removes the element from the container, but doesn't preserve the order of the elements.
	void RemoveAt(const iterator itr);

	/// \brief Removes the X element from the container whilst preserving the order of the elements.
	void RemoveOrderedAt(const int32 index);
	/// \brief Removes the X until Y elements from the container whilst preserving the order of the elements.
	void RemoveOrderedAt(const int32 startIndex, const int32 endIndex);
	/// \brief Removes the element from the container whilst preserving the order of the elements.
	void RemoveOrderedAt(const iterator itr);

	/// \brief Removes the last element from the container.
	void RemoveLast();
	/// \brief Removes the last X elements from the container.
	void RemoveLast(int32 count);

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
	std::vector<Type> m_Values = { };
};

#include "Array.inl"