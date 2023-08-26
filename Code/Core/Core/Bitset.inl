#pragma once

inline auto Bitset::operator[](const int32 index)-> std::vector<bool>::reference
{
	const size_t current = m_Values.size();
	const size_t desired = static_cast<size_t>(index) + 1;
	if (current < desired)
		m_Values.resize(desired, false);
	return m_Values[index];
}

inline auto Bitset::operator[](const int32 index) const -> bool
{
	return index < static_cast<int32>(m_Values.size())
		? m_Values[index] : false;
}