#include "ECS/ComponentMask.h"

#include "Math/Math.h"

auto ecs::ComponentMask::Iterator::operator*() -> ecs::ComponentId
{
	return ecs::ComponentId{ m_Index };
}

auto ecs::ComponentMask::Iterator::operator++() -> Iterator&
{
	while (++m_Index < COUNT && !m_Mask.Has(m_Index)) 
	{
	}
	return *this;
}

bool ecs::ComponentMask::Iterator::operator!=(const Iterator& rhs) const
{
	return m_Index != rhs.m_Index;
}

//////////////////////////////////////////////////////////////////////////

auto ecs::ComponentMask::begin() const -> Iterator
{
	Iterator itr = { *this, 0 };
	if (!Has(0))
		itr.operator++();

	return itr;
}

auto ecs::ComponentMask::end() const -> Iterator
{
	return Iterator{ *this, COUNT };
}

bool ecs::ComponentMask::operator<(const ComponentMask& rhs) const noexcept
{
	for (int16 i = CHUNKS - 1; i >= 0; --i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return m_Data[i] < rhs.m_Data[i];
	}

	return false;
}

bool ecs::ComponentMask::operator<=(const ComponentMask& rhs) const noexcept
{
	for (int16 i = CHUNKS - 1; i >= 0; --i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return m_Data[i] < rhs.m_Data[i];
	}

	return true;
}

bool ecs::ComponentMask::operator>(const ComponentMask& rhs) const noexcept
{
	for (int16 i = CHUNKS - 1; i >= 0; --i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return m_Data[i] > rhs.m_Data[i];
	}

	return false;
}

bool ecs::ComponentMask::operator>=(const ComponentMask& rhs) const noexcept
{
	for (int16 i = CHUNKS - 1; i >= 0; --i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return m_Data[i] > rhs.m_Data[i];
	}

	return true;
}

bool ecs::ComponentMask::operator==(const ComponentMask& rhs) const noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return false;
	}

	return true;
}

bool ecs::ComponentMask::operator!=(const ComponentMask& rhs) const noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
	{
		if (m_Data[i] != rhs.m_Data[i])
			return true;
	}

	return false;
}

ecs::ComponentMask ecs::ComponentMask::operator|(const ComponentMask& rhs) const noexcept
{
	ecs::ComponentMask lhs = *this;
	for (uint8 i = 0; i < CHUNKS; ++i)
		lhs.m_Data[i] |= rhs.m_Data[i];
	return lhs;
}

ecs::ComponentMask ecs::ComponentMask::operator&(const ComponentMask& rhs) const noexcept
{
	ecs::ComponentMask lhs = *this;
	for (uint8 i = 0; i < CHUNKS; ++i)
		lhs.m_Data[i] &= rhs.m_Data[i];
	return lhs;
}

ecs::ComponentMask ecs::ComponentMask::operator^(const ComponentMask& rhs) const noexcept
{
	ecs::ComponentMask lhs = *this;
	for (uint8 i = 0; i < CHUNKS; ++i)
		lhs.m_Data[i] ^= rhs.m_Data[i];
	return lhs;
}

ecs::ComponentMask ecs::ComponentMask::operator~() const noexcept
{
	ecs::ComponentMask lhs = *this;
	for (uint8 i = 0; i < CHUNKS; ++i)
		lhs.m_Data[i] = ~lhs.m_Data[i];
	return lhs;
}

ecs::ComponentMask& ecs::ComponentMask::operator|=(const ComponentMask& rhs) noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] |= rhs.m_Data[i];
	return *this;
}

ecs::ComponentMask& ecs::ComponentMask::operator&=(const ComponentMask& rhs) noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] &= rhs.m_Data[i];
	return *this;
}

ecs::ComponentMask& ecs::ComponentMask::operator^=(const ComponentMask& rhs) noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] ^= rhs.m_Data[i];
	return *this;
}

void ecs::ComponentMask::Clear(const int32 index) noexcept
{
	const uint8 chunk = index / STRIDE;
	const uint8 shift = index % STRIDE;
	const uint64 bit = BIT64(shift);
	m_Data[chunk] &= ~bit;
}

void ecs::ComponentMask::Clear(const ecs::ComponentMask& rhs) noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] &= ~rhs.m_Data[i];
}

void ecs::ComponentMask::ClearAll() noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] = 0;
}

void ecs::ComponentMask::Raise(const int32 index) noexcept
{
	const uint8 chunk = index / STRIDE;
	const uint8 shift = index % STRIDE;
	const uint64 bit = BIT64(shift);
	m_Data[chunk] |= bit;
}

void ecs::ComponentMask::Raise(const ecs::ComponentMask& rhs) noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] |= rhs.m_Data[i];
}

void ecs::ComponentMask::RaiseAll() noexcept
{
	for (uint8 i = 0; i < CHUNKS; ++i)
		m_Data[i] = UINT64_MAX;
}

bool ecs::ComponentMask::Has(const int32 index) const noexcept
{
	const uint8 chunk = index / STRIDE;
	const uint8 shift = index % STRIDE;
	const uint64 bit = BIT64(shift);
	return (m_Data[chunk] & bit) != 0;
}

bool ecs::ComponentMask::HasAll() const noexcept
{
	bool result = true;
	for (uint8 i = 0; i < CHUNKS; ++i)
		result &= (m_Data[i] == UINT64_MAX);
	return result;
}

bool ecs::ComponentMask::HasAll(const ComponentMask& rhs) const noexcept
{
	bool result = true;
	for (uint8 i = 0; i < CHUNKS; ++i)
		result &= ((m_Data[i] & rhs.m_Data[i]) == rhs.m_Data[i]);
	return result;
}

bool ecs::ComponentMask::HasAny() const noexcept
{
	bool result = false;
	for (uint8 i = 0; i < CHUNKS; ++i)
		result |= (m_Data[i] != 0);
	return result;
}

bool ecs::ComponentMask::HasAny(const ComponentMask& rhs) const noexcept
{
	bool result = false;
	for (uint8 i = 0; i < CHUNKS; ++i)
		result |= ((m_Data[i] & rhs.m_Data[i]) != 0);
	return result;
}

bool ecs::ComponentMask::HasNone() const noexcept
{
	bool result = true;
	for (uint8 i = 0; i < CHUNKS; ++i)
		result &= (m_Data[i] == 0);
	return result;
}

bool ecs::ComponentMask::HasNone(const ComponentMask& rhs) const noexcept
{
	return !HasAny(rhs);
}