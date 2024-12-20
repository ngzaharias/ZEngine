#pragma once

#include "Core/EnumHelpers.h"
#include "Core/TypeName.h"
#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Read

template<typename Value>
inline void eng::Visitor::Read(Value& value) const
{
	ReadCustom<Value>(value);
}

template<typename Key, typename Value>
inline void eng::Visitor::Read(Map<Key, Value>& values) const
{
	ReadMap(values);
}

template<typename Value>
void eng::Visitor::Read(const str::StringView& key, Value& value, const Value& defaultValue) const
{
	toml::Table& parentNode = *m_Node->as_table();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		if (toml::Array* childNode = parentNode[key].as_array())
		{
			m_Node = childNode;
			ReadArray(value);
		}
		else
		{
			value = defaultValue;
		}
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		if (toml::Table* childNode = parentNode[key].as_table())
		{
			m_Node = childNode;
			ReadMap(value);
		}
		else
		{
			value = defaultValue;
		}
	}
	else if constexpr (core::IsSpecialization<Value, Set>::value)
	{
		if (toml::Array* childNode = parentNode[key].as_array())
		{
			m_Node = childNode;
			ReadSet(value);
		}
		else
		{
			value = defaultValue;
		}
	}
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		ReadOptional(key, value, defaultValue);
	}
	// same as above
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		if (toml::Table* childNode = parentNode[key].as_table())
		{
			m_Node = childNode;
			ReadVariant(value);
		}
		else
		{
			value = defaultValue;
		}
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		ReadEnum(key, value, defaultValue);
	}
	else // custom
	{
		if (toml::Table* childNode = parentNode[key].as_table())
		{
			m_Node = childNode;
			ReadCustom(value);
		}
		else
		{
			value = defaultValue;
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
inline void eng::Visitor::Read(const int32 index, Value& value) const
{
	toml::Array& parentNode = *m_Node->as_array();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		if (toml::Array* childNode = parentNode.at(index).as_array())
		{
			m_Node = childNode;
			ReadArray(value);
		}
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		if (toml::Table* childNode = parentNode.at(index).as_table())
		{
			m_Node = childNode;
			ReadMap(value);
		}
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		if (toml::Table* childNode = parentNode.at(index).as_table())
		{
			m_Node = childNode;
			ReadVariant(value);
		}
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		ReadEnum(index, value);
	}
	else // custom
	{
		if (toml::Table* childNode = parentNode.at(index).as_table())
		{
			m_Node = childNode;
			ReadCustom(value);
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::ReadArray(Array<Value>& values) const
{
	toml::Array& parentNode = *m_Node->as_array();
	values.Resize(static_cast<int32>(parentNode.size()));
	for (int32 i = 0; i < values.GetCount(); ++i)
		Read(i, values[i]);
	m_Node = &parentNode;
}

template <typename TEnum>
void eng::Visitor::ReadEnum(const str::StringView& key, TEnum& value, const TEnum defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();

	// by string
	const auto result = currentNode[key].value<str::String>();
	value = result ? StringToEnum<TEnum>(*result) : defaultValue;

	// by value
	// using Value = std::underlying_type<TEnum>::type;
	//const auto result = currentNode[key].value<Value>();
	//value = result ? static_cast<TEnum>(*result) : defaultValue;
}

template <typename TEnum>
void eng::Visitor::ReadEnum(const int32 index, TEnum& value) const
{
	toml::Array& currentNode = *m_Node->as_array();

	// by string
	if (const auto result = currentNode.at(index).value<str::String>())
		value = StringToEnum<TEnum>(*result);

	// by value
	//using Value = std::underlying_type<TEnum>::type;
	//if (const auto result = currentNode.at(index).value<Value>())
	//	value = static_cast<TEnum>(*result);
}

template<typename Value>
void eng::Visitor::ReadMap(Map<str::Guid, Value>& values) const
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto& node : parentNode)
	{
		const str::String key = str::String(node.first.str());
		if (str::Guid::IsValidString(key))
		{
			const str::Guid guid = str::Guid::Create(key);
			auto& value = values[guid];
			Read(key, value, value);
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::ReadMap(Map<str::Name, Value>& values) const
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto& node : parentNode)
	{
		const str::String key = str::String(node.first.str());
		const str::Name name = str::Name::Create(key);
		auto& value = values[name];
		Read(key, value, value);
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::ReadMap(Map<str::String, Value>& values) const
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto& node : parentNode)
	{
		const str::String key = str::String(node.first.str());
		auto& value = values[key];
		Read(key, value, value);
	}
	m_Node = &parentNode;
}

template<typename Type>
void eng::Visitor::ReadOptional(const str::StringView& key, Optional<Type>& value, const Optional<Type>& defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	if (currentNode.contains(key))
	{
		value.emplace();
		Read(key, *value, {});
	}
	else
	{
		value = defaultValue;
	}
}

template<typename Value>
void eng::Visitor::ReadPrimitive(const str::StringView& key, Value& value, const Value defaultValue) const
{
	toml::Table& currentNode = *m_Node->as_table();
	const auto result = currentNode[key].value<Value>();
	value = result ? *result : defaultValue;
}

template<typename Value>
void eng::Visitor::ReadPrimitive(const int32 index, Value& value) const
{
	toml::Array& currentNode = *m_Node->as_array();
	if (const auto result = currentNode.at(index).value<Value>())
		value = *result;
}

template<typename Value>
void eng::Visitor::ReadSet(Set<Value>& values) const
{
	toml::Array& parentNode = *m_Node->as_array();
	const int32 count = static_cast<int32>(parentNode.size());
	for (int32 i = 0; i < count; ++i)
	{
		Value value;
		Read(i, value);
		values.Add(value);
	}
	m_Node = &parentNode;
}

template<typename ...Types>
void eng::Visitor::ReadVariant(Variant<Types...>& value) const
{
	// use a short-circuiting fold expression to assign the correct type
	((ReadVariantElement<Types>(value)) || ...);
}

template<typename TElement, typename TVariant>
bool eng::Visitor::ReadVariantElement(TVariant& value) const
{
	const str::StringView typeName = core::ToElementName<TElement>();

	toml::Table& currentNode = *m_Node->as_table();
	if (!currentNode[typeName])
		return false;

	Read(typeName, value.emplace<TElement>(), {});
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Write

template<typename Value>
inline void eng::Visitor::Write(const Value& value)
{
	WriteCustom<Value>(value);
}

template<typename Key, typename Value>
inline void eng::Visitor::Write(const Map<Key, Value>& values)
{
	WriteMap(values);
}

template<typename Value>
inline void eng::Visitor::Write(const str::StringView& key, const Value& value)
{
	toml::Table& parentNode = *m_Node->as_table();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		toml::Array childNode;
		m_Node = &childNode;
		WriteArray(value);
		parentNode.insert_or_assign(key, std::move(childNode));
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteMap(value);
		parentNode.insert_or_assign(key, std::move(childNode));
	}
	else if constexpr (core::IsSpecialization<Value, Set>::value)
	{
		toml::Array childNode;
		m_Node = &childNode;
		WriteSet(value);
		parentNode.insert_or_assign(key, std::move(childNode));
	}
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		WriteOptional(key, value);
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteVariant(value);
		parentNode.insert_or_assign(key, std::move(childNode));
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		WriteEnum(key, value);
	}
	else // custom
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteCustom(value);
		parentNode.insert_or_assign(key, std::move(childNode));
	}
	m_Node = &parentNode;
}

template<typename Value>
inline void eng::Visitor::Write(const int32 index, const Value& value)
{
	toml::Array& parentNode = *m_Node->as_array();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		toml::Array childNode;
		m_Node = &childNode;
		WriteArray(value);
		parentNode.push_back(std::move(childNode));
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteMap(value);
		parentNode.push_back(std::move(childNode));
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteVariant(value);
		parentNode.push_back(std::move(childNode));
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		WriteEnum(index, value);
	}
	else // custom
	{
		toml::Table childNode;
		m_Node = &childNode;
		WriteCustom(value);
		parentNode.push_back(std::move(childNode));
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::WriteArray(const Array<Value>& values)
{
	toml::Array& parentNode = *m_Node->as_array();
	parentNode.reserve(static_cast<size_t>(values.GetCount()));
	for (int32 i = 0; i < values.GetCount(); ++i)
		Write(i, values[i]);
	m_Node = &parentNode;
}

template <typename TEnum>
void eng::Visitor::WriteEnum(const str::StringView& key, const TEnum& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, EnumToString(value)); // by string
	// currentNode.insert_or_assign(key, static_cast<int64>(value)); // by value
}

template <typename TEnum>
void eng::Visitor::WriteEnum(const int32 index, const TEnum& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(EnumToString(value)); // by string
	// currentNode.push_back(static_cast<int64>(value)); // by value
}

template<typename Value>
void eng::Visitor::WriteMap(const Map<str::Guid, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto&& [guid, value] : values)
	{
		const str::String key = guid.ToString();
		Write(key, value, value);
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::WriteMap(const Map<str::Name, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto&& [key, value] : values)
		Write(key, value, value);
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::WriteMap(const Map<str::String, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	for (auto&& [key, value] : values)
		Write(key, value);
	m_Node = &parentNode;
}

template<typename Type>
void eng::Visitor::WriteOptional(const str::StringView& key, const Optional<Type>& value)
{
	if (value)
		Write(key, *value);
}

template<typename Value>
void eng::Visitor::WritePrimitive(const str::StringView& key, const Value& value)
{
	toml::Table& currentNode = *m_Node->as_table();
	currentNode.insert_or_assign(key, value);
}

template<typename Value>
void eng::Visitor::WritePrimitive(const int32 index, const Value& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	currentNode.push_back(value);
}

template<typename Value>
void eng::Visitor::WriteSet(const Set<Value>& values)
{
	int32 i = 0;
	toml::Array& parentNode = *m_Node->as_array();
	parentNode.reserve(static_cast<size_t>(values.GetCount()));
	for (const Value& value : values)
		Write(i++, value);
	m_Node = &parentNode;
}

template<typename ...Types>
void eng::Visitor::WriteVariant(const Variant<Types...>& value)
{
	core::VariantMatch(value, [&](auto& element)
		{
			const str::StringView typeName = core::ToElementName(element);
			Write(typeName, element);
		});
}