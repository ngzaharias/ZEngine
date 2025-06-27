#pragma once

#include "Core/Slice.h"

#include <array>

// https://stackoverflow.com/questions/7758580/writing-your-own-stl-container

using int32 = int32_t;

template<typename Type, int32 TCount>
class StaticArray
{
	static_assert(TCount > 0, "StaticArray must be at least a size of 1.");

public:
	using Iterator = typename std::array<Type, TCount>::iterator;
	using CIterator = typename std::array<Type, TCount>::const_iterator;

public:
	constexpr StaticArray() noexcept;
	constexpr StaticArray(const Type& value) noexcept;
	constexpr StaticArray(StaticArray&& rhs) noexcept;
	constexpr StaticArray(const StaticArray& rhs) noexcept;
	constexpr StaticArray(std::initializer_list<Type>&& range) noexcept;

	constexpr auto operator=(StaticArray&& rhs) noexcept -> StaticArray&;
	constexpr auto operator=(const StaticArray& rhs) noexcept -> StaticArray&;
	constexpr auto operator=(std::initializer_list<Type>&& rhs) noexcept -> StaticArray&;

	constexpr auto operator[](const int32 index) noexcept->Type&;
	constexpr auto operator[](const int32 index) const noexcept -> const Type&;

	/// \brief Implicit conversion to Slice.
	operator Slice<Type>();
	/// \brief Implicit conversion to Slice.
	operator Slice<const Type>() const;

	/// \brief Gets the number of elements in the container.
	constexpr int32 GetCount() const noexcept;

	/// \brief Get a raw pointer to the data container.
	constexpr Type* GetData() noexcept;
	/// \brief Get a raw pointer to the data container.
	constexpr const Type* GetData() const noexcept;

	/// \brief Gets the first element from the container.
	constexpr auto GetFirst() noexcept->Type&;
	/// \brief Gets the first element from the container.
	constexpr auto GetFirst() const noexcept -> const Type&;

	/// \brief Gets the last element from the container.
	constexpr auto GetLast() noexcept->Type&;
	/// \brief Gets the last element from the container.
	constexpr auto GetLast() const noexcept -> const Type&;

public:
	constexpr auto begin() const { return m_Values.begin(); }
	constexpr auto rbegin() { return m_Values.rbegin(); }

	constexpr auto end() { return m_Values.end(); }
	constexpr auto rend() { return m_Values.rend(); }

	constexpr auto begin() { return m_Values.begin(); }
	constexpr auto cbegin() const { return m_Values.cbegin(); }
	constexpr auto rbegin() const { return m_Values.rbegin(); }
	constexpr auto crbegin() const { return m_Values.crbegin(); }

	constexpr auto end() const { return m_Values.end(); }
	constexpr auto cend() const { return m_Values.cend(); }
	constexpr auto rend() const { return m_Values.rend(); }
	constexpr auto crend() const { return m_Values.crend(); }

private:
	// #todo: make C-Style Array
	std::array<Type, TCount> m_Values = { };
};

#include "StaticArray.inl"