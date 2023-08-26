#include "Core/Name.h"

#include "Core/NameTable.h"

str::Name str::Name::Unassigned = str::Name();

str::Name::operator str::StringView() const
{
	return NameTable::Instance().Retrieve(m_Hash);
}

const bool str::Name::IsEmpty() const
{
	return m_Hash == UINT32_MAX;
}

const char* str::Name::ToChar() const
{
	return NameTable::Instance().Retrieve(m_Hash).c_str();
}

const std::string& str::Name::ToString() const
{
	return NameTable::Instance().Retrieve(m_Hash);
}

str::Name str::Name::Create(const str::StringView& string)
{
	Name name;
	if (!string.empty())
		name.m_Hash = NameTable::Instance().Register(string);
#ifdef Z_DEBUG
	name.m_Debug = &NameTable::Instance().Retrieve(name.m_Hash);
#endif
	return name;
}
