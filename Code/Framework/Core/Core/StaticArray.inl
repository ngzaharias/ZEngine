#pragma once

namespace _private
{
	template<typename Type, std::size_t N, std::size_t ...Ns>
	constexpr std::array<Type, N> Make(const Type& value, std::index_sequence<Ns...>)
	{
		return { {(static_cast<void>(Ns), value)...} };
	}

	template<typename Type, std::size_t N, std::size_t ...Ns>
	constexpr std::array<Type, N> Make(std::initializer_list<Type>& range, std::index_sequence<Ns...>)
	{
		return std::array<Type, N>{ *(range.begin() + Ns) ... };
	}
}

template<typename Type, int32 TCount>
constexpr StaticArray<Type, TCount>::StaticArray() noexcept
	: m_Values()
{
}

template<typename Type, int32 TCount>
constexpr StaticArray<Type, TCount>::StaticArray(const Type& value) noexcept
	: m_Values(_private::Make<Type, TCount>(value, std::make_index_sequence<TCount>()))
{
}

template<typename Type, int32 TCount>
constexpr StaticArray<Type, TCount>::StaticArray(StaticArray&& rhs) noexcept
	: m_Values(std::move(rhs.m_Values))
{
}

template<typename Type, int32 TCount>
constexpr StaticArray<Type, TCount>::StaticArray(const StaticArray& rhs) noexcept
	: m_Values(rhs.m_Values)
{
}

template<typename Type, int32 TCount>
constexpr StaticArray<Type, TCount>::StaticArray(std::initializer_list<Type>&& range) noexcept
	: m_Values(_private::Make<Type, TCount>(range, std::make_index_sequence<TCount>()))
{
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::operator=(StaticArray&& rhs) noexcept -> StaticArray&
{
	m_Values = std::move(rhs.m_Values);
	return *this;
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::operator=(const StaticArray& rhs) noexcept -> StaticArray&
{
	m_Values = rhs.m_Values;
	return *this;
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::operator=(std::initializer_list<Type>&& rhs) noexcept -> StaticArray&
{
	m_Values = _private::Make<Type, TCount>(rhs.m_Values, std::make_index_sequence<TCount>());
	return *this;
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::operator [](const int32 index) noexcept->Type&
{
	return m_Values[index];
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::operator [](const int32 index) const noexcept -> const Type&
{
	return m_Values[index];
}

template<typename Type, int32 TCount>
StaticArray<Type, TCount>::operator Slice<Type>()
{
	return Slice<Type>(GetData(), GetCount());
}

template<typename Type, int32 TCount>
StaticArray<Type, TCount>::operator Slice<const Type>() const
{
	return Slice<const Type>(GetData(), GetCount());
}

template<typename Type, int32 TCount>
constexpr int32 StaticArray<Type, TCount>::GetCount() const noexcept
{
	return TCount;
}

template<typename Type, int32 TCount>
constexpr Type* StaticArray<Type, TCount>::GetData() noexcept
{
	return &m_Values[0];
}

template<typename Type, int32 TCount>
constexpr const Type* StaticArray<Type, TCount>::GetData() const noexcept
{
	return &m_Values[0];
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::GetFirst() noexcept->Type&
{
	return m_Values[0];
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::GetFirst() const noexcept -> const Type&
{
	return m_Values[0];
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::GetLast() noexcept->Type&
{
	return m_Values.back();
}

template<typename Type, int32 TCount>
constexpr auto StaticArray<Type, TCount>::GetLast() const noexcept -> const Type&
{
	return m_Values.back();
}