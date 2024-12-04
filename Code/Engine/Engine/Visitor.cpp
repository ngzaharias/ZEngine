#include "EnginePCH.h"
#include "Engine/Visitor.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace eng
{
	std::ostream& operator<<(std::ostream& lhs, const eng::Visitor& rhs)
	{
		lhs << rhs.m_Root;
		return lhs;
	}
}

//////////////////////////////////////////////////////////////////////////

eng::Visitor::Iterator::Iterator(Itr itr, Node** node)
	: m_Itr(itr)
	, m_Node(node)
	, m_Parent(*node)
{
}

eng::Visitor::Iterator::~Iterator()
{
	*m_Node = m_Parent;
}

auto eng::Visitor::Iterator::operator*() -> str::StringView
{
	*m_Node = &m_Itr->second;
	return m_Itr->first.str();
}

auto eng::Visitor::Iterator::operator++() -> Iterator&
{
	m_Itr++;
	return *this;
}

bool eng::Visitor::Iterator::operator!=(const Iterator& other) const
{
	return m_Itr != other.m_Itr;
}

//////////////////////////////////////////////////////////////////////////

eng::Visitor::Visitor(Visitor&& value) noexcept
	: m_Root(std::move(value.m_Root))
	, m_Node(&m_Root)
{
}

eng::Visitor::Visitor(const str::StringView& value)
	: m_Node(&m_Root)
{
	if (toml::parse_result result = toml::parse(value))
		m_Root = std::move(result).table();
}

eng::Visitor& eng::Visitor::operator=(Visitor&& rhs) noexcept
{
	m_Root = std::move(rhs.m_Root);
	return *this;
}

eng::Visitor& eng::Visitor::operator=(const str::Path& rhs) noexcept
{
	if (toml::parse_result result = toml::parse_file(rhs.ToView()))
	{
		m_Root = std::move(result).table();
	}
	else
	{
		m_Root.clear();
	}
	return *this;
}

eng::Visitor& eng::Visitor::operator=(const str::StringView& rhs) noexcept
{
	if (toml::parse_result result = toml::parse(rhs))
	{
		m_Root = std::move(result).table();
	}
	else
	{
		m_Root.clear();
	}
	return *this;
}

eng::Visitor::operator str::String() noexcept
{
	//std::stringstream stream;
	//stream << m_Root;
	//return stream.str();

	std::stringstream stream;
	m_Node->visit([&stream](const auto& n) { stream << n; });
	return stream.str();
}

void eng::Visitor::SetInline()
{
	if (toml::Table* currentNode = m_Node->as_table())
		currentNode->is_inline(true);
}

bool eng::Visitor::SaveToFile(const str::Path& filepath)
{
	std::ofstream file;
	file.open(filepath.ToChar(), std::fstream::out);
	if (!file.is_open())
		return false;

	file << toml::toml_formatter{ m_Root };
	return true;
}

bool eng::Visitor::LoadFromFile(const str::Path& filepath)
{
	PROFILE_FUNCTION();

	toml::parse_result result = toml::parse_file(filepath.ToView());
	if (!result)
		return false;

	m_Root = std::move(result).table();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Read - Visit by Key

void eng::Visitor::Read(const str::StringView& key, bool& value, const bool defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, float& value, const float defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, double& value, const double defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, int8& value, const int8 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, int16& value, const int16 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, int32& value, const int32 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, int64& value, const int64 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, uint8& value, const uint8 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, uint16& value, const uint16 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, uint32& value, const uint32 defaultValue) const
{
	ReadPrimitive(key, value, defaultValue);
}

void eng::Visitor::Read(const str::StringView& key, str::Guid& value, const str::Guid& defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	const auto result = currentNode[key].value<str::String>();
	value = result ? GUID(*result) : defaultValue;
}

void eng::Visitor::Read(const str::StringView& key, str::Name& value, const str::Name& defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	const auto result = currentNode[key].value<str::String>();
	value = result ? NAME(*result) : defaultValue;
}

void eng::Visitor::Read(const str::StringView& key, str::Path& value, const str::Path& defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	const auto result = currentNode[key].value<str::String>();
	value = result ? str::Path(*result) : defaultValue;
}

void eng::Visitor::Read(const str::StringView& key, str::String& value, const str::String& defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	const auto result = currentNode[key].value<str::String>();
	value = result ? *result : defaultValue;
}

//////////////////////////////////////////////////////////////////////////
// Read - Visit by Index

void eng::Visitor::Read(const int32 index, bool& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, float& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, double& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, int8& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, int16& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, int32& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, int64& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, uint8& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, uint16& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, uint32& value) const
{
	ReadPrimitive(index, value);
}

void eng::Visitor::Read(const int32 index, str::Guid& value) const
{
	toml::Array& currentNode = *m_Node->as_array();
	if (const auto result = currentNode.at(index).value<str::String>())
		value = GUID(*result);
}

void eng::Visitor::Read(const int32 index, str::Name& value) const
{
	toml::Array& currentNode = *m_Node->as_array();
	if (const auto result = currentNode.at(index).value<str::String>())
		value = NAME(*result);
}

void eng::Visitor::Read(const int32 index, str::Path& value) const
{
	toml::Array& currentNode = *m_Node->as_array();
	if (const auto result = currentNode.at(index).value<str::String>())
		value = str::Path(*result);
}

void eng::Visitor::Read(const int32 index, str::String& value) const
{
	toml::Array& currentNode = *m_Node->as_array();
	if (const auto result = currentNode.at(index).value<str::String>())
		value = *result;
}

//////////////////////////////////////////////////////////////////////////
// Read - Non-Fundamentals

template<>
void eng::Visitor::ReadCustom(Quaternion& value) const
{
	Read("X", value.x, 0.f);
	Read("Y", value.y, 0.f);
	Read("Z", value.z, 0.f);
	Read("W", value.w, 0.f);
}

template<>
void eng::Visitor::ReadCustom(Rotator& value) const
{
	Read("Pitch", value.m_Pitch, 0.f);
	Read("Yaw", value.m_Yaw, 0.f);
	Read("Roll", value.m_Roll, 0.f);
}

template<>
void eng::Visitor::ReadCustom(Vector2f& value) const
{
	Read("X", value.x, 0.f);
	Read("Y", value.y, 0.f);
}

template<>
void eng::Visitor::ReadCustom(Vector2i& value) const
{
	Read("X", value.x, 0);
	Read("Y", value.y, 0);
}

template<>
void eng::Visitor::ReadCustom(Vector2u& value) const
{
	Read("X", value.x, 0u);
	Read("Y", value.y, 0u);
}

template<>
void eng::Visitor::ReadCustom(Vector3f& value) const
{
	Read("X", value.x, 0.f);
	Read("Y", value.y, 0.f);
	Read("Z", value.z, 0.f);
}

template<>
void eng::Visitor::ReadCustom(Vector3i& value) const
{
	Read("X", value.x, 0);
	Read("Y", value.y, 0);
	Read("Z", value.z, 0);
}

template<>
void eng::Visitor::ReadCustom(Vector4f& value) const
{
	Read("X", value.x, 0.f);
	Read("Y", value.y, 0.f);
	Read("Z", value.z, 0.f);
	Read("W", value.w, 0.f);
}

//////////////////////////////////////////////////////////////////////////
// Write - Visit by Key

void eng::Visitor::Write(const str::StringView& key, const bool& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const float& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const double& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const int8& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const int16& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const int32& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const int64& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const uint8& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const uint16& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const uint32& value)
{
	WritePrimitive(key, value);
}

void eng::Visitor::Write(const str::StringView& key, const str::Guid& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, value.ToString());
}

void eng::Visitor::Write(const str::StringView& key, const str::Name& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, str::StringView(value));
}

void eng::Visitor::Write(const str::StringView& key, const str::Path& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, str::StringView(value));
}

void eng::Visitor::Write(const str::StringView& key, const str::String& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, value);
}

//////////////////////////////////////////////////////////////////////////
// Write - Visit by Index

void eng::Visitor::Write(const int32 index, const bool& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const float& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const double& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const int8& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const int16& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const int32& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const int64& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const uint8& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const uint16& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const uint32& value)
{
	WritePrimitive(index, value);
}

void eng::Visitor::Write(const int32 index, const str::Guid& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(value.ToString());
}

void eng::Visitor::Write(const int32 index, const str::Name& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(str::StringView(value));
}

void eng::Visitor::Write(const int32 index, const str::Path& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(str::StringView(value));
}

void eng::Visitor::Write(const int32 index, const str::String& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(value);
}

//////////////////////////////////////////////////////////////////////////
// Write - Non-Fundamentals

template<>
void eng::Visitor::WriteCustom(const Quaternion& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
	Write("Z", value.z);
	Write("W", value.w);
}

template<>
void eng::Visitor::WriteCustom(const Rotator& value)
{
	SetInline();
	Write("Pitch", value.m_Pitch);
	Write("Yaw", value.m_Yaw);
	Write("Roll", value.m_Roll);
}

template<>
void eng::Visitor::WriteCustom(const Vector2f& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
}

template<>
void eng::Visitor::WriteCustom(const Vector2i& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
}

template<>
void eng::Visitor::WriteCustom(const Vector2u& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
}

template<>
void eng::Visitor::WriteCustom(const Vector3f& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
	Write("Z", value.z);
}

template<>
void eng::Visitor::WriteCustom(const Vector3i& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
	Write("Z", value.z);
}

template<>
void eng::Visitor::WriteCustom(const Vector4f& value)
{
	SetInline();
	Write("X", value.x);
	Write("Y", value.y);
	Write("Z", value.z);
	Write("W", value.w);
}

//////////////////////////////////////////////////////////////////////////

auto eng::Visitor::begin()->Iterator
{
	toml::Table* table = m_Node->as_table();
	return Iterator(table->begin(), &m_Node);
}

auto eng::Visitor::end()->Iterator
{
	toml::Table* table = m_Node->as_table();
	return Iterator(table->end(), &m_Node);
}