#pragma once

#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

template<typename Key, typename Value>
void eng::Visitor::Visit(Map<Key, Value>& values)
{
	VisitMap(values);
}

template<typename Value>
void eng::Visitor::Visit(const str::StringView& key, Value& value, const Value& defaultValue)
{
	toml::Table& parentNode = *m_Node->as_table();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		if (IsReading())
		{
			if (toml::Array* childNode = parentNode[key].as_array())
			{
				m_Node = childNode;
				VisitArray(value);
			}
			else
			{
				value = defaultValue;
			}
		}
		else
		{
			toml::Array childNode;
			m_Node = &childNode;
			VisitArray(value);
			parentNode.insert_or_assign(key, std::move(childNode));
		}
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		if (IsReading())
		{
			if (toml::Table* childNode = parentNode[key].as_table())
			{
				m_Node = childNode;
				VisitMap(value);
			}
			else
			{
				value = defaultValue;
			}
		}
		else
		{
			toml::Table childNode;
			m_Node = &childNode;
			VisitMap(value);
			parentNode.insert_or_assign(key, std::move(childNode));
		}
	}
	// #todo: unsure why, but checking for specialization doesn't work if the type is wrapped with the `using` alias
	// so instead we have to check the actual type rather than our friendly version.
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		if (IsReading())
		{
			if (toml::Table* childNode = parentNode[key].as_table())
			{
				m_Node = childNode;
				VisitVariant(value);
			}
			else
			{
				value = defaultValue;
			}
		}
		else
		{
			toml::Table childNode;
			m_Node = &childNode;
			VisitVariant(value);
			parentNode.insert_or_assign(key, std::move(childNode));
		}
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		VisitEnum(key, value, defaultValue);
	}
	else // custom
	{
		if (IsReading())
		{
			if (toml::Table* childNode = parentNode[key].as_table())
			{
				m_Node = childNode;
				VisitCustom(value);
			}
			else
			{
				value = defaultValue;
			}
		}
		else
		{
			toml::Table childNode;
			m_Node = &childNode;
			VisitCustom(value);
			parentNode.insert_or_assign(key, std::move(childNode));
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::Visit(const int32 index, Value& value)
{
	toml::Array& parentNode = *m_Node->as_array();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		if (IsReading())
		{
			if (toml::Array* childNode = parentNode.at(index).as_array())
			{
				m_Node = childNode;
				VisitArray(value);
			}
		}
		else
		{
			toml::Array childNode;
			m_Node = &childNode;
			VisitArray(value);
			parentNode.push_back(std::move(childNode));
		}
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		if (IsReading())
		{
			if (toml::Table* childNode = parentNode.at(index).as_array())
			{
				m_Node = childNode;
				VisitMap(value);
			}
		}
		else
		{
			toml::Table childNode;
			m_Node = &childNode;
			VisitMap(value);
			parentNode.push_back(std::move(childNode));
		}
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		VisitEnum(index, value);
	}
	else // custom
	{
		if (IsReading())
		{
			if (toml::Table* childNode = parentNode.at(index).as_table())
			{
				m_Node = childNode;
				VisitCustom(value);
			}
		}
		else
		{
			toml::Table childNode;
			m_Node = &childNode;
			VisitCustom(value);
			parentNode.push_back(std::move(childNode));
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::VisitArray(Array<Value>& values)
{
	toml::Array& parentNode = *m_Node->as_array();
	if (IsReading())
	{
		values.Resize(static_cast<int32>(parentNode.size()));
		for (int32 i = 0; i < values.GetCount(); ++i)
			Visit(i, values[i]);
	}
	else
	{
		parentNode.reserve(static_cast<size_t>(values.GetCount()));
		for (int32 i = 0; i < values.GetCount(); ++i)
			Visit(i, values[i]);
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::VisitMap(Map<str::Guid, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	if (IsReading())
	{
		for (auto& node : parentNode)
		{
			const str::String key = str::String(node.first.str());
			if (str::Guid::IsValidString(key))
			{
				const str::Guid guid = str::Guid::Create(key);
				auto& value = values[guid];
				Visit(key, value, value);
			}
		}
	}
	else
	{
		for (auto&& [guid, value] : values)
		{
			const str::String key = guid.ToString();
			Visit(key, value, value);
		}
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::VisitMap(Map<str::Name, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	if (IsReading())
	{
		for (auto& node : parentNode)
		{
			const str::String key = str::String(node.first.str());
			const str::Name name = str::Name::Create(key);
			auto& value = values[name];
			Visit(key, value, value);
		}
	}
	else
	{
		for (auto&& [key, value] : values)
			Visit(key, value, value);
	}
	m_Node = &parentNode;
}

template<typename Value>
void eng::Visitor::VisitMap(Map<str::String, Value>& values)
{
	toml::Table& parentNode = *m_Node->as_table();
	if (IsReading())
	{
		for (auto& node : parentNode)
		{
			const str::String key = str::String(node.first.str());
			auto& value = values[key];
			Visit(key, value, value);
		}
	}
	else
	{
		for (auto&& [key, value] : values)
			Visit(key, value, value);
	}
	m_Node = &parentNode;
}

template <typename TEnum>
void eng::Visitor::VisitEnum(const str::StringView& key, TEnum& value, const TEnum defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		using Value = std::underlying_type<TEnum>::type;
		const auto result = currentNode[key].value<Value>();
		value = result ? static_cast<TEnum>(*result) : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, static_cast<int64>(value));
	}
}

template <typename TEnum>
void eng::Visitor::VisitEnum(const int32 index, TEnum& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		using Value = std::underlying_type<TEnum>::type;
		if (const auto result = currentNode.at(index).value<Value>())
			value = static_cast<TEnum>(*result);
	}
	else
	{
		currentNode.push_back(static_cast<int64>(value));
	}
}

template<typename Value>
void eng::Visitor::VisitPrimitive(const str::StringView& key, Value& value, const Value defaultValue)
{
	toml::Table& currentNode = *m_Node->as_table();
	if (IsReading())
	{
		const auto result = currentNode[key].value<Value>();
		value = result ? *result : defaultValue;
	}
	else
	{
		currentNode.insert_or_assign(key, value);
	}
}

template<typename Value>
void eng::Visitor::VisitPrimitive(const int32 index, Value& value)
{
	toml::Array& currentNode = *m_Node->as_array();
	if (IsReading())
	{
		if (const auto result = currentNode.at(index).value<Value>())
			value = *result;
	}
	else
	{
		currentNode.push_back(value);
	}
}

template<typename ...Types>
void eng::Visitor::VisitVariant(Variant<Types...>& value)
{
	constexpr str::StringView strTypeId = "TypeId";
	constexpr str::StringView strTypeVal = "TypeVal";

	if (IsReading())
	{
		int32 type, index = 0;
		Visit(strTypeId, type, 0);

		// use a short-circuiting fold expression to assign the correct type
		((index == type
			? (value.template emplace<Types>(), true)
			: (index++, false)) || ...);

		core::VariantMatch(value, [&](auto& element)
			{
				Visit(strTypeVal, element, {});
			});
	}
	else
	{
		int32 type = (int32)value.index();
		Visit(strTypeId, type, 0);
		core::VariantMatch(value, [&](auto& element)
			{
				Visit(strTypeVal, element, {});
			});
	}
}