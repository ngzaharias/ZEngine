#include "Core/Address.h"

void str::Address::Push(const str::StringView& string)
{
	uint32 length = static_cast<uint32>(m_Full.length());
	if (length > 0)
		m_Full += '/';
	m_Full += str::String(string);
	m_Positions.Append(length);
}

void str::Address::Pop()
{
	if (!m_Positions.IsEmpty())
	{
		const uint32 last = m_Positions.GetLast();
		m_Positions.RemoveLast();
		m_Full.erase(last);
	}
}

str::StringView str::Address::GetFull() const
{
	return m_Full;
}

str::StringView str::Address::GetLast() const
{
	const uint32 count = static_cast<uint32>(m_Full.length());
	const uint32 prev = m_Positions.GetCount() > 1 ? m_Positions.GetLast() + 1 : 0;
	return str::StringView(m_Full).substr(prev, count - prev);
}