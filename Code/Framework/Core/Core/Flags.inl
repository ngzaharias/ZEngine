#pragma once

#include <limits>

template<typename Enum>
constexpr Flags<Enum>::Flags(const Enum flag)
	: m_Value(Value(flag))
{
}

template<typename Enum>
constexpr Flags<Enum>::Flags(const Flags& flags)
	: m_Value(flags.m_Value)
{
}

template<typename Enum>
constexpr bool Flags<Enum>::Has(const Enum flag) const
{
	return (m_Value & flag) != 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasAll(const Flags& flags) const
{
	return (m_Value & flags.m_Value) == flags.m_Value;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasAny() const
{
	return m_Value != 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasAny(const Flags& flags) const
{
	return (m_Value & flags.m_Value) != 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasOnly(const Enum flag) const
{
	return (m_Value & ~flag) == 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasOnly(const Flags& flags) const
{
	return (m_Value & ~flags.m_Value) == 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasNone() const
{
	return m_Value == 0;
}

template<typename Enum>
constexpr bool Flags<Enum>::HasNone(const Flags& flags) const
{
	return (m_Value & flags.m_Value) == 0;
}

template<typename Enum>
constexpr void Flags<Enum>::Clear(const Enum flag)
{
	m_Value &= ~flag;
}


template<typename Enum>
constexpr void Flags<Enum>::Clear(const Flags& flags)
{
	m_Value &= ~flags.m_Value;
}

template<typename Enum>
constexpr void Flags<Enum>::ClearAll()
{
	m_Value = 0;
}

template<typename Enum>
constexpr void Flags<Enum>::Raise(const Enum flag)
{
	m_Value |= flag;
}


template<typename Enum>
constexpr void Flags<Enum>::Raise(const Flags& flags)
{
	m_Value |= flags.m_Value;
}

template<typename Enum>
constexpr void Flags<Enum>::RaiseAll()
{
	m_Value = std::numeric_limits<Value>::max();
}

template<typename Enum>
constexpr void Flags<Enum>::Set(const Enum flag, const bool value)
{
	value ? Raise(flag) : Clear(flag);
}

template<typename Enum>
constexpr void Flags<Enum>::Toggle(const Enum flag)
{
	m_Value ^= flag;
}

