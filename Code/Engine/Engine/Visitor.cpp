#include "EnginePCH.h"
#include "Engine/Visitor.h"

#include <Core/Guid.h>
#include <Core/Name.h>
#include <Core/Path.h>
#include <Core/Quaternion.h>
#include <Core/String.h>
#include <Core/Vector.h>

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
	, m_Mode(value.m_Mode)
{
}

eng::Visitor::Visitor(const str::StringView& value)
	: m_Mode(EMode::Write)
	, m_Node(&m_Root)
{
	if (toml::parse_result result = toml::parse(value))
		m_Root = std::move(result).table();
}

eng::Visitor& eng::Visitor::operator=(Visitor&& rhs) noexcept
{
	m_Root = std::move(rhs.m_Root);
	m_Mode = rhs.m_Mode;
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

	file << m_Root;
	return true;
}

bool eng::Visitor::LoadFromFile(const str::Path& filepath)
{
	PROFILE_FUNCTION();

	toml::parse_result result = toml::parse_file(filepath.ToView());
	if (!result)
		return false;

	m_Mode = EMode::Read;
	m_Root = std::move(result).table();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Visit by Key

void eng::Visitor::Visit(const str::StringView& key, bool& value, const bool defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, float& value, const float defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, double& value, const double defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, int8& value, const int8 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, int16& value, const int16 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, int32& value, const int32 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, int64& value, const int64 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, uint8& value, const uint8 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, uint16& value, const uint16 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, uint32& value, const uint32 defaultValue)
{
	VisitPrimitive(key, value, defaultValue);
}

void eng::Visitor::Visit(const str::StringView& key, str::Guid& value, const str::Guid& defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		const auto result = currentNode[key].value<str::String>();
		value = result ? GUID(*result) : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, value.ToString());
	}
}

void eng::Visitor::Visit(const str::StringView& key, str::Name& value, const str::Name& defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		const auto result = currentNode[key].value<str::String>();
		value = result ? NAME(*result) : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, str::StringView(value));
	}
}

void eng::Visitor::Visit(const str::StringView& key, str::Path& value, const str::Path& defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		const auto result = currentNode[key].value<str::String>();
		value = result ? str::Path(*result) : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, str::StringView(value));
	}
}

void eng::Visitor::Visit(const str::StringView& key, str::String& value, const str::String& defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		const auto result = currentNode[key].value<str::String>();
		value = result ? *result : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, value);
	}
}

//////////////////////////////////////////////////////////////////////////
// Visit by Index

void eng::Visitor::Visit(const int32 index, bool& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, float& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, double& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, int8& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, int16& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, int32& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, int64& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, uint8& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, uint16& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, uint32& value)
{
	VisitPrimitive(index, value);
}

void eng::Visitor::Visit(const int32 index, str::Guid& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		if (const auto result = currentNode.at(index).value<str::String>())
			value = GUID(*result);
	}
	else
	{
		currentNode.push_back(value.ToString());
	}
}

void eng::Visitor::Visit(const int32 index, str::Name& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		if (const auto result = currentNode.at(index).value<str::String>())
			value = NAME(*result);
	}
	else
	{
		currentNode.push_back(str::StringView(value));
	}
}

void eng::Visitor::Visit(const int32 index, str::Path& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		if (const auto result = currentNode.at(index).value<str::String>())
			value = str::Path(*result);
	}
	else
	{
		currentNode.push_back(str::StringView(value));
	}
}

void eng::Visitor::Visit(const int32 index, str::String& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		if (const auto result = currentNode.at(index).value<str::String>())
			value = *result;
	}
	else
	{
		currentNode.push_back(value);
	}
}

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

//////////////////////////////////////////////////////////////////////////
// Non-Fundamentals

template<>
void eng::Visitor::VisitCustom<Quaternion>(Quaternion& value)
{
	SetInline();
	Visit("X", value.x, 0.f);
	Visit("Y", value.y, 0.f);
	Visit("Z", value.z, 0.f);
	Visit("W", value.w, 0.f);
}

template<>
void eng::Visitor::VisitCustom<Rotator>(Rotator& value)
{
	SetInline();
	Visit("Pitch", value.m_Pitch, 0.f);
	Visit("Yaw", value.m_Yaw, 0.f);
	Visit("Roll", value.m_Roll, 0.f);
}

template<>
void eng::Visitor::VisitCustom<Vector2f>(Vector2f& value)
{
	SetInline();
	Visit("X", value.x, 0.f);
	Visit("Y", value.y, 0.f);
}

template<>
void eng::Visitor::VisitCustom<Vector2i>(Vector2i& value)
{
	SetInline();
	Visit("X", value.x, 0);
	Visit("Y", value.y, 0);
}

template<>
void eng::Visitor::VisitCustom<Vector2u>(Vector2u& value)
{
	SetInline();
	Visit("X", value.x, 0);
	Visit("Y", value.y, 0);
}

template<>
void eng::Visitor::VisitCustom<Vector3f>(Vector3f& value)
{
	SetInline();
	Visit("X", value.x, 0.f);
	Visit("Y", value.y, 0.f);
	Visit("Z", value.z, 0.f);
}

template<>
void eng::Visitor::VisitCustom<Vector3i>(Vector3i& value)
{
	SetInline();
	Visit("X", value.x, 0);
	Visit("Y", value.y, 0);
	Visit("Z", value.z, 0);
}

template<>
void eng::Visitor::VisitCustom<Vector4f>(Vector4f& value)
{
	SetInline();
	Visit("X", value.x, 0.f);
	Visit("Y", value.y, 0.f);
	Visit("Z", value.z, 0.f);
	Visit("W", value.w, 0.f);
}