#include "Core/NameTable.h"

#include <assert.h>

namespace
{
	str::String strEmpty;
}

str::NameTable& str::NameTable::Instance()
{
	if (!s_Instance)
		s_Instance = new NameTable();
	return *s_Instance;
}

str::NameTable::NameTable()
{
	s_Instance = this;
}

str::NameTable::~NameTable()
{
	s_Instance = nullptr;
}

str::Hash str::NameTable::Register(const str::StringView& string)
{
	// #fixme: shouldn't need to convert to string
	const str::Hash hash = str::ToHash(string);
	const auto find = m_Values.Find(hash);
	if (find == m_Values.end())
		m_Values[hash] = string;
	return hash;
}

const str::String& str::NameTable::Retrieve(const str::Hash& hash) const
{
	const auto find = m_Values.Find(hash);
	if (find != m_Values.end())
		return find->second;
	return strEmpty;
}
