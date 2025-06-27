#include "Core/Path.h"

#include "Core/EnumHelpers.h"
#include "Core/StaticArray.h"

namespace
{
	constexpr int32 s_Count = EnumToCount<str::EPath>();
	static StaticArray<str::Path, s_Count> s_Paths = {};
}

str::Path::Path()
	: m_Value()
{
}

str::Path::Path(str::String&& value)
	: m_Value(std::move(value))
{
}

str::Path::Path(const char* value)
	: m_Value(value)
{
}

str::Path::Path(const str::String& value)
	: m_Value(value)
{
}

str::Path::Path(const str::StringView& value)
	: m_Value(value)
{
}

str::Path::Path(const str::EPath type)
	: m_Value()
{
	*this = GetPath(type);
}

bool str::Path::operator==(const str::Path& rhs) const
{
	return m_Value == rhs.m_Value;
}

bool str::Path::operator==(const str::StringView& rhs) const
{
	return m_Value == rhs;
}

bool str::Path::operator!=(const str::Path& rhs) const
{
	return m_Value != rhs.m_Value;
}

bool str::Path::operator!=(const str::StringView& rhs) const
{
	return m_Value != rhs;
}

bool str::Path::operator<(const str::Path& rhs) const
{
	return m_Value < rhs.m_Value;
}

void str::Path::operator=(str::String&& rhs)
{
	m_Value = std::move(rhs);
}

void str::Path::operator=(const str::Path& rhs)
{
	m_Value = rhs.m_Value;
}

void str::Path::operator=(const str::String& rhs)
{
	m_Value = str::String(rhs);
}

void str::Path::operator=(const str::StringView& rhs)
{
	m_Value = str::String(rhs);
}

void str::Path::operator+=(const str::StringView& rhs)
{
	m_Value += str::String(rhs);
}

bool str::Path::HasExtension() const
{
	if (IsEmpty())
		return false;

	const auto find = m_Value.find_last_of('.', m_Value.size());
	return find != std::string::npos;
}

bool str::Path::IsDirectory() const
{
	if (IsEmpty())
		return false;

	const auto find = m_Value.find_last_of('.', m_Value.size());
	return find == std::string::npos;
}

str::StringView str::Path::GetDirectory() const
{
	const auto findDot = m_Value.find_last_of('.', m_Value.size());
	const auto findSlash = m_Value.find_last_of("/\\", m_Value.size());

	// Directory/Foo.bar
	if (findDot != std::string::npos && findSlash != std::string::npos)
		return str::StringView(m_Value.c_str(), findSlash + 1);

	// Foo.bar
	if (findDot != std::string::npos && findSlash == std::string::npos)
		return { };

	// Directory/Foo/
	return m_Value;
}

str::StringView str::Path::GetFileExtension() const
{
	const size_t size = m_Value.size();
	const auto find = m_Value.find_last_of('.', size);
	if (find != std::string::npos)
		return str::StringView(m_Value.c_str() + find, size - find);
	return { };
}

str::StringView str::Path::GetFileName() const
{
	const size_t size = m_Value.size();
	const auto find = m_Value.find_last_of("/\\", m_Value.size());
	if (find != std::string::npos)
		return str::StringView(m_Value.c_str() + find + 1, size - find - 1);
	return m_Value;
}

str::StringView str::Path::GetFileNameNoExtension() const
{
	const auto findDot = m_Value.find_last_of('.', m_Value.size());
	const auto findSlash = m_Value.find_last_of("/\\", m_Value.size());

	// Directory/Foo.bar
	if (findDot != std::string::npos && findSlash != std::string::npos)
		return str::StringView(m_Value.c_str() + findSlash + 1, findDot - findSlash - 1);

	// Foo.bar
	if (findDot != std::string::npos && findSlash == std::string::npos)
		return str::StringView(m_Value.c_str(), findDot);
	return { };
}

str::StringView str::Path::GetParent() const
{
	const size_t size = m_Value.size();

	const auto find = m_Value.find_last_of("/\\", m_Value.size() - 2);
	if (find != std::string::npos && find + 1 < size)
		return str::StringView(m_Value.c_str(), find);

	return { };
}

str::StringView str::Path::GetStem() const
{
	const size_t size = m_Value.size();
	const auto find = m_Value.find_last_of("/\\", size);
	if (find != std::string::npos)
		return str::StringView(m_Value.c_str() + find + 1, size - find - 1);

	return m_Value;
}

void str::Path::Clear()
{
	m_Value.clear();
}

const str::Path& str::GetPath(const EPath type)
{
	const int32 index = EnumToValue(type);
	return s_Paths[index];
}

void str::SetPath(const EPath type, const str::Path& value)
{
	const int32 index = EnumToValue(type);
	s_Paths[index] = value;
}