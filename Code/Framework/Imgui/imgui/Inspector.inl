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
void imgui::Inspector::Read(const Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	ReadMember(value);
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
inline bool imgui::Inspector::Write(Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	return WriteMember(value);
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

template<class TPayload>
void imgui::Inspector::AddPayload(TPayload& payload)
{
	using NonConst = std::remove_const<TPayload>::type;
	Z_PANIC(!HasPayload<NonConst>(), "Payload has already been added!");

	const int32 payloadId = ToTypeIndex<NonConst, imgui::InspectorTag>();
	m_Payload.Emplace(payloadId, (void*)&payload);
}

template<class TPayload>
bool imgui::Inspector::HasPayload()
{
	using NonConst = std::remove_const<TPayload>::type;
	const int32 payloadId = ToTypeIndex<NonConst, imgui::InspectorTag>();
	return m_Payload.Contains(payloadId);
}

template<class TPayload>
auto imgui::Inspector::GetPayload()->TPayload&
{
	using NonConst = std::remove_const<TPayload>::type;
	Z_PANIC(HasPayload<NonConst>(), "Payload hasn't been added!");

	const int32 payloadId = ToTypeIndex<NonConst, imgui::InspectorTag>();
	return *static_cast<NonConst*>(m_Payload.Get(payloadId));
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
	else if constexpr (core::IsSpecialization<Value, std::variant>::value)
	{
		bool isExpanded = false;
		str::String preview = {};
		core::VariantMatch(value, [&](const auto& element)
			{
				isExpanded = detail::Header(label, element);
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
		return detail::Header<Value>(label);
	}
}

template<typename Value>
bool imgui::Inspector::WriteHeader(const char* label, Value& value)
{
	RaiiID id(label);
	if constexpr (core::IsSpecialization<Value, Array>::value)
	{
		struct Append {};
		struct PopBack {};
		struct RemoveAll {};
		using Command = Optional<Variant<Append, PopBack, RemoveAll>>;

		Command command = {};
		bool isExpanded = detail::Header<Value>(label);
		ImGui::OpenPopupOnItemClick("array##parent");

		if (ImGui::BeginPopup("array##parent"))
		{
			if (ImGui::Selectable("Append"))
				command = Append{};
			if (ImGui::Selectable("PopBack"))
				command = PopBack{};
			if (ImGui::Selectable("RemoveAll"))
				command = RemoveAll{};
			ImGui::EndPopup();
		}

		if (command)
		{
			core::VariantMatch(*command,
				[&value](const Append&) { value.Emplace(); },
				[&value](const PopBack&) { if (!value.IsEmpty()) { value.Pop(); } },
				[&value](const RemoveAll&) { value.RemoveAll(); });
		}

		return isExpanded;
	}
	else if constexpr (core::IsSpecialization<Value, std::optional>::value)
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

		if (hasValue)
			return WriteHeader(label, *value);
		return detail::Header<Value::value_type>(label);
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
			detail::ComboVariant(value);
			ImGui::EndCombo();
		}

		return detail::Header<Value>(label);
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
	struct Insert { int32 i; };
	struct Remove { int32 i; };
	using Command = Optional<Variant<Insert, Remove>>;

	bool result = false;
	Command command = {};
	for (int32 i = 0; i < values.GetCount(); ++i)
	{
		const str::String label = std::to_string(i);
		ImGui::TableNextRow();

		RaiiID id(label);
		bool isExpanded = WriteHeader(label.c_str(), values[i]);
		ImGui::OpenPopupOnItemClick("array##child");

		if (isExpanded)
		{
			RaiiIndent indent(0);
			ImGui::TableSetColumnIndex(1);
			result |= WriteMember(values[i]);
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

	return result;
}

template <typename Value>
void imgui::Inspector::ReadEnum(const Value& value)
{
	ImGui::SetNextItemWidth(-1);

	RaiiDisable disable;
	const str::String string = str::String(EnumToString(value));
	if (ImGui::BeginCombo("##value", string.c_str()))
		ImGui::EndCombo();
}

template <typename Value>
bool imgui::Inspector::WriteEnum(Value& value)
{
	bool result = false;
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
	core::VariantMatch(value, [&](const auto& element)
		{
			Read("##value", element);
		});
}


template<typename ...Values>
bool imgui::Inspector::WriteVariant(Variant<Values...>& value)
{
	bool result = false;
	core::VariantMatch(value, [&](auto& element)
		{
			result = Write("##value", element);
		});
	return result;
}