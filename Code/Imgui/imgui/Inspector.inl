#pragma once

#include "Core/EnumHelpers.h"
#include "Core/TypeName.h"
#include "Core/TypeTraits.h"
#include "Core/VariantHelpers.h"

template<> struct imgui::Inspector::IsCollapsable<bool> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<float> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<double> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<int8> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<int16> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<int32> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<int64> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<uint8> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<uint16> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<uint32> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<uint64> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Quaternion> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Rotator> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector2f> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector2i> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector2u> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector3f> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector3i> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<Vector4f> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<str::Guid> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<str::Name> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<str::Path> : std::false_type {};
template<> struct imgui::Inspector::IsCollapsable<str::String> : std::false_type {};

template<typename Value>
void imgui::Inspector::Read(const char* label, const Value& value)
{
	ImGui::TableSetColumnIndex(0);
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
	else
	{
		ReadCustom(value);
	}
	ImGui::TableNextRow();
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

template <typename Value>
void imgui::Inspector::ReadEnum(const Value& value)
{
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

template<typename Value>
void imgui::Inspector::ReadOptional(const Optional<Value>& value)
{
}

template<typename ...Values>
void imgui::Inspector::ReadVariant(const Variant<Values...>& value)
{
}

template<typename Value>
inline bool imgui::Inspector::Write(const char* label, Value& value)
{
	RaiiID id(label);
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);

	bool result = false;
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen))
		{
			RaiiIndent indent(0);
			result = WriteArray(value);
		}
	}
	else if constexpr (core::IsSpecialization<Value, Map>::value)
	{
		if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen))
		{
			RaiiIndent indent(0);
			result = WriteMap(value);
		}
	}
	// #todo: unsure why, but checking for specialization doesn't work if the type is wrapped with the `using` alias
	// so instead we have to check the actual type rather than our friendly version.
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
	{
		bool hasValue = value.has_value();
		ImGui::TableSetColumnIndex(2);
		if (ImGui::Checkbox("##enable", &hasValue))
		{
			result = true;
			if (hasValue) { value.emplace(); }
			else { value.reset(); }
		}

		ImGui::TableSetColumnIndex(0);
		if constexpr (imgui::Inspector::IsCollapsable<Value::value_type>::value)
		{
			if (ImGui::CollapsingHeader(label))
			{
				RaiiIndent indent(0);
				result |= WriteOptional(value);
			}
		}
		else
		{
			ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);
			ImGui::TableSetColumnIndex(1);
			result |= WriteOptional(value);
		}
	}
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		if (ImGui::CollapsingHeader(label))
		{
			RaiiIndent indent(0);
			result = WriteVariant(value);
		}
	}
	else if constexpr (std::is_enum<Value>::value)
	{
		ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);
		ImGui::TableSetColumnIndex(1);
		result = WriteEnum(value);
	}
	else if constexpr (imgui::Inspector::IsCollapsable<Value>::value)
	{
		if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen))
		{
			RaiiIndent indent(0);
			result = WriteCustom(value);
		}
	}
	else
	{
		ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_Bullet);
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		result = WriteCustom(value);
	}

	return result;
}

template<typename Value>
bool imgui::Inspector::WriteArray(Array<Value>& values)
{
	bool result = false;
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String string = std::format("{}", i);
		result |= Write(string.c_str(), values[i]);
	}
	return result;
}

template <typename Value>
bool imgui::Inspector::WriteEnum(Value& value)
{
	bool result = false;
	return result;
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
bool imgui::Inspector::WriteOptional(Optional<Value>& value)
{
	bool result = false;
	ImGui::TableSetColumnIndex(1);
	if (value)
	{
		if constexpr (core::IsSpecialization<Value, Array>::value)
		{
			result |= WriteArray(*value);
		}
		else if constexpr (core::IsSpecialization<Value, Map>::value)
		{
			result |= WriteMap(*value);
		}
		// #todo: unsure why, but checking for specialization doesn't work if the type is wrapped with the `using` alias
		// so instead we have to check the actual type rather than our friendly version.
		else if constexpr (core::IsSpecialization<Value, std::optional>::value)
		{
			static_assert(false, "Optional<Optional<...>> is not supported!");
		}
		else if constexpr (core::IsSpecialization<Value, std::variant>::value)
		{
			result |= WriteVariant(*value);
		}
		else if constexpr (std::is_enum<Value>::value)
		{
			result |= WriteEnum(*value);
		}
		else
		{
			result |= WriteCustom(*value);
		}
	}
	else
	{
		ImGui::Text("...");
	}
	return result;
}

template<typename ...Values>
bool imgui::Inspector::WriteVariant(Variant<Values...>& value)
{
	bool result = false;
	return result;
}