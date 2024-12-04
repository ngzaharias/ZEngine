#pragma once

#include "Core/Types.h"

template<typename Type>
class Slice
{
public:
	Slice();
	Slice(Type* begin, const int32 count);
	Slice(Type* begin, Type* end);
	Slice(const Slice& rhs);

	auto operator=(const Slice& rhs)->Slice&;

	auto operator[](const int32 index)->Type&;
	auto operator[](const int32 index) const -> const Type&;

	/// \brief Converts to a const version of itself.
	operator Slice<const Type>() const;

	/// \brief Checks if the container is empty and if the slice is null.
	bool IsEmpty() const;
	/// \brief Checks if the container is pointing to anything.
	bool IsNull() const;
	/// \brief Gets the number of elements in the container.
	int32 GetCount() const;

	/// \brief Get a raw pointer to the data container.
	Type* GetData();
	/// \brief Get a raw pointer to the data container.
	const Type* GetData() const;

	/// \brief Gets the first element from the container.
	auto GetFirst()->Type&;
	/// \brief Gets the first element from the container.
	auto GetFirst() const -> const Type&;

	/// \brief Gets the last element from the container.
	auto GetLast()->Type&;
	/// \brief Gets the last element from the container.
	auto GetLast() const -> const Type&;

public:
	auto begin() { return m_Begin; }
	auto begin() const { return m_Begin; }
	auto cbegin() const { return m_Begin; }

	auto end() { return m_End; }
	auto end() const { return m_End; }
	auto cend() const { return m_End; }

private:
	Type* m_Begin;
	Type* m_End;
};

#include "Slice.inl"