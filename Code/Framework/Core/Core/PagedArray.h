#pragma once

#include "Core/Array.h"

using int32 = int32_t;

template<typename Type, int32 Exponent>
class PagedArray
{
	using Page = Array<Type>;
	using Pages = Array<Page>;

	static_assert(Exponent > 0, "Exponent must be at least a size of 1.");

	static constexpr int32 PageSize = 1 << Exponent;
	static constexpr int32 PageMask = PageSize - 1;

public:
	auto operator[](const int32 index)->Type&;
	auto operator[](const int32 index) const -> const Type&;

	int32 GetPage(const int32 index) const;
	int32 GetOffset(const int32 index) const;

	/// \brief Appends a new element to the back of the container.
	auto Append(Type&& value)->Type&;
	/// \brief Appends a new element to the back of the container.
	auto Append(const Type& value)->Type&;
	/// \brief Appends a range of elements to the back of the container.
	void Append(PagedArray&& range);
	/// \brief Appends a range of elements to the back of the container.
	void Append(const PagedArray& range);
	/// \brief Appends a range of elements to the back of the container.
	void Append(std::initializer_list<Type>&& range);

	template<typename... Args>
	Type& Emplace(Args&& ...args);

	/// \brief Inserts a new element at a specific index in the container.
	/// Assumes that the index is within the containers current bounds.
	auto Insert(const int32 index, Type&& value)->Type&;
	/// \brief Inserts a new element at a specific index in the container.
	/// Assumes that the index is within the containers current bounds.
	auto Insert(const int32 index, const Type& value)->Type&;

private:
	Pages m_Pages = { };
};

#include "PagedArray.inl"