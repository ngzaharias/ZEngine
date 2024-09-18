#pragma once

#include "Core/Algorithms.h"
#include "Core/EnumHelpers.h"
#include "Core/TypeName.h"
#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

template<> struct imgui::Inspector::IsInline<bool> : std::true_type {};
template<> struct imgui::Inspector::IsInline<float> : std::true_type {};
template<> struct imgui::Inspector::IsInline<double> : std::true_type {};
template<> struct imgui::Inspector::IsInline<int8> : std::true_type {};
template<> struct imgui::Inspector::IsInline<int16> : std::true_type {};
template<> struct imgui::Inspector::IsInline<int32> : std::true_type {};
template<> struct imgui::Inspector::IsInline<int64> : std::true_type {};
template<> struct imgui::Inspector::IsInline<uint8> : std::true_type {};
template<> struct imgui::Inspector::IsInline<uint16> : std::true_type {};
template<> struct imgui::Inspector::IsInline<uint32> : std::true_type {};
template<> struct imgui::Inspector::IsInline<uint64> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Quaternion> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Rotator> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector2f> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector2i> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector2u> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector3f> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector3i> : std::true_type {};
template<> struct imgui::Inspector::IsInline<Vector4f> : std::true_type {};
template<> struct imgui::Inspector::IsInline<str::Guid> : std::true_type {};
template<> struct imgui::Inspector::IsInline<str::Name> : std::true_type {};
template<> struct imgui::Inspector::IsInline<str::Path> : std::true_type {};
template<> struct imgui::Inspector::IsInline<str::String> : std::true_type {};

namespace detail
{
	template<typename Value>
	inline bool Header(const char* label)
	{
		constexpr bool isEnum = std::is_enum<Value>::value;
		constexpr bool isInline = imgui::Inspector::IsInline<Value>::value;
		if constexpr (isEnum || isInline)
		{
			ImGui::TableSetColumnIndex(0);
			ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);
			return true;
		}
		else
		{
			ImGui::TableSetColumnIndex(0);
			return ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen);
		}
	}
}

template<typename Value>
void imgui::Inspector::Read(const char* label, const Value& value)
{
	ImGui::TableNextRow();

	RaiiID id(label);
	if (ReadHeader(label, value))
	{
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		ReadMember(value);
	}
}

template<typename Value>
inline bool imgui::Inspector::Write(const char* label, Value& value)
{
	ImGui::TableNextRow();

	RaiiID id(label);
	if (WriteHeader(label, value))
	{
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		return WriteMember(value);
	}
	return false;
}

template<typename Value>
bool imgui::Inspector::ReadHeader(const char* label, const Value& value)
{
	if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		bool hasValue = value.has_value();
		ImGui::TableSetColumnIndex(2);
		imgui::Checkbox("##enable", hasValue);

		return detail::Header<Value::value_type>(label);
	}
	else
	{
		return detail::Header<Value>(label);
	}
}

template<typename Value>
bool imgui::Inspector::WriteHeader(const char* label, Value& value)
{
	if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		bool hasValue = value.has_value();
		ImGui::TableSetColumnIndex(2);
		if (imgui::Checkbox("##enable", hasValue))
		{
			if (hasValue)
				value.emplace();
			if (!hasValue)
				value.reset();
		}

		return detail::Header<Value::value_type>(label);
	}
	else
	{
		return detail::Header<Value>(label);
	}
	return false;
}

template<typename Value>
void imgui::Inspector::ReadMember(const Value& value)
{
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		ReadArray(value);
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		ReadMap(value);
	}
	else if constexpr (core::IsSpecialization<Value, Set>::value)
	{
		ReadSet(value);
	}
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		ReadOptional(value);
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		ReadVariant(value);
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		ReadEnum(value);
	}
	else
	{
		ReadCustom(value);
	}
}

template<typename Value>
bool imgui::Inspector::WriteMember(Value& value)
{
	bool result = false;
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		result = WriteArray(value);
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		result = WriteMap(value);
	}
	else if constexpr (core::IsSpecialization<Value, Set>::value)
	{
		result = WriteSet(value);
	}
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
	else
	{
		result = WriteCustom(value);
	}
	return result;
}

template<typename Value>
void imgui::Inspector::ReadArray(const Array<Value>& values)
{
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String label = std::to_string(i);
		ImGui::TableNextRow();

		RaiiID id(label.c_str());
		if (ReadHeader(label.c_str(), values[i]))
		{
			RaiiIndent indent(0);
			ImGui::TableSetColumnIndex(1);
			ReadMember(values[i]);
		}
	}
}

template<typename Value>
bool imgui::Inspector::WriteArray(Array<Value>& values)
{
	bool result = false;

	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String label = std::to_string(i);
		ImGui::TableNextRow();

		RaiiID id(label.c_str());
		bool isExpanded = WriteHeader(label.c_str(), values[i]);
		ImGui::OpenPopupOnItemClick("##popup");

		if (isExpanded)
		{
			RaiiIndent indent(0);
			ImGui::TableSetColumnIndex(1);
			result |= WriteMember(values[i]);
		}
	}

	if (ImGui::BeginPopup("##popup"))
	{
		if (ImGui::Selectable("Append"))
			values.Emplace();
		if (ImGui::Selectable("PopBack"))
			values.Pop();
		if (ImGui::Selectable("RemoveAll"))
			values.RemoveAll();

		ImGui::EndPopup();
	}

	return result;
}

template <typename Value>
void imgui::Inspector::ReadEnum(const Value& value)
{
	RaiiDisable disable;
	const str::String string = str::String(EnumToString(value));
	if (ImGui::BeginCombo("##value", string.c_str()))
		ImGui::EndCombo();
}

template <typename Value>
bool imgui::Inspector::WriteEnum(Value& value)
{
	bool result = false;
	ImGui::TableSetColumnIndex(1);

	const str::String preview = str::String(EnumToString(value));
	if (ImGui::BeginCombo("##value", preview.c_str()))
	{
		const int32 count = EnumToCount<Value>();
		for (int32 i = 0; i < count; ++i)
		{
			const Value item = IndexToEnum<Value>(i);
			const str::String string = str::String(EnumToString(item));
			if (ImGui::Selectable(string.c_str(), value == item))
			{
				result = true;
				value = item;
			}
		}
		ImGui::EndCombo();
	}
	return result;
}

template<typename Key, typename Value>
void imgui::Inspector::ReadMap(const Map<Key, Value>& values)
{
	for (auto&& [key, value] : values)
	{
		const str::String string = std::format("{}", key);
		Read(string.c_str(), value);
	}
}

template<typename Key, typename Value>
bool imgui::Inspector::WriteMap(Map<Key, Value>& values)
{
	bool result = false;
	for (auto&& [key, value] : values)
	{
		const str::String string = std::format("{}", key);
		result |= Write(string.c_str(), value);
	}
	return result;
}

template<typename Value>
void imgui::Inspector::ReadOptional(const Optional<Value>& value)
{
	if (value)
	{
		ReadMember(*value);
	}
	else
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("...");
	}
}

template<typename Value>
bool imgui::Inspector::WriteOptional(Optional<Value>& value)
{
	bool result = false;
	if (value)
	{
		result = WriteMember(*value);
	}
	else
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("...");
	}
	return result;
}

template<typename Value>
void imgui::Inspector::ReadSet(const Set<Value>& values)
{
	for (auto&& [index, value] : enumerate::Forward(values))
	{
		const str::String string = std::format("{}", index);
		Read(string.c_str(), value);
	}
}

template<typename Value>
bool imgui::Inspector::WriteSet(Set<Value>& values)
{
	for (auto&& [index, value] : enumerate::Forward(values))
	{
		const str::String string = std::format("{}", index);
		Read(string.c_str(), value);
	}
	return false;
}

template<typename ...Values>
void imgui::Inspector::ReadVariant(const Variant<Values...>& value)
{
}

template<typename ...Values>
bool imgui::Inspector::WriteVariant(Variant<Values...>& value)
{
	bool result = false;
	return result;
}