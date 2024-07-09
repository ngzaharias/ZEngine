#pragma once

#include "Core/Types.h"

#include <vector>

class Bitset
{
public:
	inline auto operator[](const int32 index)->std::vector<bool>::reference;
	inline auto operator[](const int32 index) const -> bool;

public:
	// #todo: iterate over only values that are set
	auto begin() const { return m_Values.begin(); }
	auto rbegin() { return m_Values.rbegin(); }

	auto end() { return m_Values.end(); }
	auto rend() { return m_Values.rend(); }

	auto begin() { return m_Values.begin(); }
	auto cbegin() const { return m_Values.cbegin(); }
	auto rbegin() const { return m_Values.rbegin(); }
	auto crbegin() const { return m_Values.crbegin(); }

	auto end() const { return m_Values.end(); }
	auto cend() const { return m_Values.cend(); }
	auto rend() const { return m_Values.rend(); }
	auto crend() const { return m_Values.crend(); }

private:
	std::vector<bool> m_Values;
};

#include "Bitset.inl"