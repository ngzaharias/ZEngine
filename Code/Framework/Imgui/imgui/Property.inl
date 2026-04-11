#pragma once

#include "Core/Algorithms.h"
#include "Core/EnumHelpers.h"
#include "Core/TypeInfo.h"
#include "Core/TypeName.h"
#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

template<> struct imgui::IsInline<bool> : std::true_type {};
template<> struct imgui::IsInline<float> : std::true_type {};
template<> struct imgui::IsInline<double> : std::true_type {};
template<> struct imgui::IsInline<int8> : std::true_type {};
template<> struct imgui::IsInline<int16> : std::true_type {};
template<> struct imgui::IsInline<int32> : std::true_type {};
template<> struct imgui::IsInline<int64> : std::true_type {};
template<> struct imgui::IsInline<uint8> : std::true_type {};
template<> struct imgui::IsInline<uint16> : std::true_type {};
template<> struct imgui::IsInline<uint32> : std::true_type {};
template<> struct imgui::IsInline<uint64> : std::true_type {};
template<> struct imgui::IsInline<Quaternion> : std::true_type {};
template<> struct imgui::IsInline<Rotator> : std::true_type {};
template<> struct imgui::IsInline<Vector2f> : std::true_type {};
template<> struct imgui::IsInline<Vector2i> : std::true_type {};
template<> struct imgui::IsInline<Vector2u> : std::true_type {};
template<> struct imgui::IsInline<Vector3f> : std::true_type {};
template<> struct imgui::IsInline<Vector3i> : std::true_type {};
template<> struct imgui::IsInline<Vector4f> : std::true_type {};
template<> struct imgui::IsInline<str::Guid> : std::true_type {};
template<> struct imgui::IsInline<str::Name> : std::true_type {};
template<> struct imgui::IsInline<str::Path> : std::true_type {};
template<> struct imgui::IsInline<str::String> : std::true_type {};

namespace _private
{
	template<typename Value>
	inline bool Header(const char* label)
	{
		constexpr bool isEnum = std::is_enum<Value>::value;
		constexpr bool isInline = imgui::IsInline<Value>::value;
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

	template<typename Value>
	inline bool Header(const char* label, const Value&)
	{
		return Header<Value>(label);
	}

	template<typename Value, typename TVariant>
	inline bool ComboElement(TVariant& value)
	{
		const str::String string = str::String(core::ToElementName<Value>());
		if (ImGui::Selectable(string.c_str()))
		{
			value.emplace<Value>();
			return true;
		}
		return false;
	}

	template<typename ...Values>
	inline bool ComboVariant(Variant<Values...>& value)
	{
		return (ComboElement<Values>(value) || ...);
	}
}

template<typename Value>
void imgui::Read(const Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	ReadMember(value);
}

template<typename Value>
void imgui::Read(const char* label, const Value& value)
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
inline bool imgui::Write(Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	return WriteMember(value);
}

template<typename Value>
inline bool imgui::Write(const char* label, Value& value)
{
	ImGui::TableNextRow();

	RaiiID id(label);
	bool modified = false;
	modified |= WriteDetails(value);
	if (WriteHeader(label, value))
	{
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		modified |= WriteMember(value);
	}
	return modified;
}

//////////////////////////////////////////////////////////////////////////

template<typename Value>
bool imgui::ReadHeader(const char* label, const Value& value)
{
	if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		bool hasValue = value.has_value();
		ImGui::TableSetColumnIndex(2);
		imgui::Checkbox("##enable", hasValue);

		return _private::Header<Value::value_type>(label);
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		bool isExpanded = false;
		str::String preview = {};
		core::VariantMatch(value, [&](const auto& element)
			{
				isExpanded = _private::Header(label, element);
				preview = str::String(core::ToElementName(element));
			});

		RaiiDisable disable;
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-1);
		if (ImGui::BeginCombo("##variant", preview.c_str()))
			ImGui::EndCombo();

		return isExpanded;
	}
	else
	{
		return _private::Header<Value>(label);
	}
}

template<typename Value>
bool imgui::WriteHeader(const char* label, Value& value)
{
	RaiiID id(label);
	if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		if (value.has_value())
			return WriteHeader(label, *value);
		return _private::Header<Value::value_type>(label);
	}
	else
	{
		return _private::Header<Value>(label);
	}
	return false;
}

template<typename Value>
bool imgui::WriteDetails(Value& value)
{
	bool modified = false;
	RaiiID id(&modified);
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		ImGui::TableSetColumnIndex(1);
		if (ImGui::Button("Append"))
		{
			modified |= true;
			value.Emplace();
		}
		ImGui::SameLine();
		if (ImGui::Button("PopBack") && !value.IsEmpty())
		{
			modified |= true;
			value.Pop();
		}
		ImGui::SameLine();
		if (ImGui::Button("RemoveAll"))
		{
			modified |= true;
			value.RemoveAll();
		}
	}
	if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		bool hasValue = value.has_value();
		ImGui::TableSetColumnIndex(2);
		if (imgui::Checkbox("##enable", hasValue))
		{
			modified |= true;
			if (hasValue)
				value.emplace();
			if (!hasValue)
				value.reset();
		}
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		str::String preview = {};
		core::VariantMatch(value, [&](const auto& element)
			{
				preview = str::String(core::ToElementName(element));
			});

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-1);
		if (ImGui::BeginCombo("##variant", preview.c_str()))
		{
			modified |= _private::ComboVariant(value);
			ImGui::EndCombo();
		}
	}
	return modified;
}

template<typename Value>
void imgui::ReadMember(const Value& value)
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
bool imgui::WriteMember(Value& value)
{
	bool modified = false;
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		modified |= WriteArray(value);
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		modified |= WriteMap(value);
	}
	else if constexpr (core::IsSpecialization<Value, Set>::value)
	{
		modified |= WriteSet(value);
	}
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		modified |= WriteOptional(value);
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		modified |= WriteVariant(value);
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		modified |= WriteEnum(value);
	}
	else
	{
		modified |= WriteCustom(value);
	}
	return modified;
}

//////////////////////////////////////////////////////////////////////////

template<typename Value>
void imgui::ReadArray(const Array<Value>& values)
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
bool imgui::WriteArray(Array<Value>& values)
{
	struct Insert { int32 i; };
	struct Remove { int32 i; };
	using Command = Optional<Variant<Insert, Remove>>;

	bool modified = false;
	Command command = {};
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String label = std::to_string(i);
		ImGui::TableNextRow();

		RaiiID id(label);
		bool isExpanded = WriteHeader(label.c_str(), values[i]);
		ImGui::OpenPopupOnItemClick("array##child");

		//modified |= WriteDetails(values[i]);
		if (isExpanded)
		{
			RaiiIndent indent(0);
			ImGui::TableSetColumnIndex(1);
			modified |= WriteMember(values[i]);
		}

		if (ImGui::BeginPopup("array##child"))
		{
			if (ImGui::Selectable("Insert /\\"))
				command = Insert{ i };
			if (ImGui::Selectable("Remove ||"))
				command = Remove{ i };
			if (ImGui::Selectable("Insert \\/"))
				command = Insert{ i + 1 };
			ImGui::EndPopup();
		}
	}

	if (command)
	{
		core::VariantMatch(*command,
			[&values](const Insert& data) { values.Insert(Value{}, data.i);	},
			[&values](const Remove& data) { values.RemoveOrderedAt(data.i); });
	}

	return modified;
}

template<typename Value>
void imgui::ReadEnum(const Value& value)
{
	ImGui::SetNextItemWidth(-1);

	RaiiDisable disable;
	const str::String string = str::String(EnumToString(value));
	if (ImGui::BeginCombo("##value", string.c_str()))
		ImGui::EndCombo();
}

template<typename Value>
bool imgui::WriteEnum(Value& value)
{
	bool modified = false;
	ImGui::SetNextItemWidth(-1);

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
				modified |= true;
				value = item;
			}
		}
		ImGui::EndCombo();
	}
	return modified;
}

template<typename Key, typename Value>
void imgui::ReadMap(const Map<Key, Value>& values)
{
	for (auto&& [key, value] : values)
	{
		const str::String string = std::format("{}", key);
		Read(string.c_str(), value);
	}
}

template<typename Key, typename Value>
bool imgui::WriteMap(Map<Key, Value>& values)
{
	bool modified = false;
	for (auto&& [key, value] : values)
	{
		const str::String string = std::format("{}", key);
		modified |= Write(string.c_str(), value);
	}
	return modified;
}

template<typename Value>
void imgui::ReadOptional(const Optional<Value>& value)
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
bool imgui::WriteOptional(Optional<Value>& value)
{
	bool modified = false;
	if (value)
	{
		modified |= WriteMember(*value);
	}
	else
	{
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("...");
	}
	return modified;
}

template<typename Value>
void imgui::ReadSet(const Set<Value>& values)
{
	for (auto&& [index, value] : enumerate::Forward(values))
	{
		const str::String string = std::format("{}", index);
		Read(string.c_str(), value);
	}
}

template<typename Value>
bool imgui::WriteSet(Set<Value>& values)
{
	for (auto&& [index, value] : enumerate::Forward(values))
	{
		const str::String string = std::format("{}", index);
		Read(string.c_str(), value);
	}
	return false;
}

template<typename ...Values>
void imgui::ReadVariant(const Variant<Values...>& value)
{
	core::VariantMatch(value, [&](const auto& element)
		{
			Read("##value", element);
		});
}


template<typename ...Values>
bool imgui::WriteVariant(Variant<Values...>& value)
{
	bool modified = false;
	modified |= WriteDetails(value);
	core::VariantMatch(value, [&](auto& element)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(1);
			modified |= WriteMember(element);
		});

	return modified;
}