#pragma once

#include "Core/EnumHelpers.h"
#include "Core/TypeName.h"
#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Read

template<typename Value>
void imgui::Inspector::Read(const char* label, const Value& value)
{
	RaiiID id(label);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	if (!ImGui::CollapsingHeader(label))
		return;

	ImGui::Indent();
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		ReadArray(value);
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		ReadMap(value);
	}
	// #todo: unsure why, but checking for specialization doesn't work if the type is wrapped with the `using` alias
	// so instead we have to check the actual type rather than our friendly version.
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		ReadOptional(value);
	}
	// same as above
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		ReadVariant(value);
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		ReadEnum(value);
	}
	else // custom
	{
		ReadCustom(value);
	}

	ImGui::TableSetColumnIndex(0);
	ImGui::Unindent();
}

template<typename Value>
void imgui::Inspector::ReadArray(const Array<Value>& values)
{
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String string = std::to_string(i);
		Read(string.c_str(), values[i]);
	}
}

template <typename TEnum>
void imgui::Inspector::ReadEnum(const TEnum& value)
{
}

template<typename Value>
void imgui::Inspector::ReadMap(const Map<str::Guid, Value>& values)
{
	for (auto&& [key, value] : values)
	{
		const str::String string = key.ToString();
		Read(string.c_str(), value);
	}
}

template<typename Value>
void imgui::Inspector::ReadMap(const Map<str::Name, Value>& values)
{
	for (auto&& [key, value] : values)
		Read(key.ToChar(), value);
}

template<typename Value>
void imgui::Inspector::ReadMap(const Map<str::String, Value>& values)
{
	for (auto&& [key, value] : values)
		Read(key.c_str(), value);
}

template<typename Type>
void imgui::Inspector::ReadOptional(const Optional<Type>& value)
{
}

template<typename ...Types>
void imgui::Inspector::ReadVariant(const Variant<Types...>& value)
{
}

//////////////////////////////////////////////////////////////////////////
// Write

template<typename Value>
inline bool imgui::Inspector::Write(const char* label, Value& value)
{
	bool result = false;
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	if (!ImGui::CollapsingHeader(label))
		return result;

	ImGui::Indent();
	ImGui::PushID(label);
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		result = WriteArray(value);
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		result = WriteMap(value);
	}
	// #todo: unsure why, but checking for specialization doesn't work if the type is wrapped with the `using` alias
	// so instead we have to check the actual type rather than our friendly version.
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		result = WriteOptional(value);
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		result = WriteVariant(value);
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		result = WriteEnum(value);
	}
	else // custom
	{
		result = WriteCustom(value);
	}
	ImGui::PopID();

	ImGui::TableSetColumnIndex(0);
	ImGui::Unindent();

	return result;
}

template<typename Value>
bool imgui::Inspector::WriteArray(Array<Value>& values)
{
	bool result = false;
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String string = std::to_string(i);
		result |= Write(string.c_str(), values[i]);
	}
	return result;
}

template <typename TEnum>
bool imgui::Inspector::WriteEnum(TEnum& value)
{
	bool result = false;
	return result;
}

template<typename Value>
bool imgui::Inspector::WriteMap(Map<str::Guid, Value>& values)
{
	bool result = false;
	for (auto&& [key, value] : values)
	{
		const str::String string = key.ToString();
		result |= Write(string.c_str(), value);
	}
	return result;
}

template<typename Value>
bool imgui::Inspector::WriteMap(Map<str::Name, Value>& values)
{
	bool result = false;
	for (auto&& [key, value] : values)
		result |= Write(key.ToChar(), value);
	return result;
}

template<typename Value>
bool imgui::Inspector::WriteMap(Map<str::String, Value>& values)
{
	bool result = false;
	for (auto&& [key, value] : values)
		result |= Write(key.c_str(), value);
	return result;
}

template<typename Type>
bool imgui::Inspector::WriteOptional(Optional<Type>& value)
{
	bool result = false;
	return result;
}

template<typename ...Types>
bool imgui::Inspector::WriteVariant(Variant<Types...>& value)
{
	bool result = false;
	return result;
}