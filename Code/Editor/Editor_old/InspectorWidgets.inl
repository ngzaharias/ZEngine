#pragma once

#include "Editor/InspectorWidgets.h"

#include "Editor/ContainerWidgets.h"
#include "Editor/TrivialWidgets.h"
#include "Editor/VariantWidgets.h"

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <refl/refl.hpp>

namespace
{
	// #note: must be declared after the function
	template <typename A, typename = void>
	struct HasOverload : std::false_type {};
	template <typename A>
	struct HasOverload<A, std::void_t<decltype(editor::TypeOverload(std::declval<A>()))>> : std::true_type {};
}

template<typename Type>
void editor::InspectType(Type& value)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&>();
	if constexpr (isOverloaded)
	{
		editor::TypeOverload(value);
	}
	else if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			editor::InspectProperty(field(value));
			ImGui::TableNextRow();
		});
	}
	else
	{
		ImGui::Text("Unsupported Type!");
	}
}

template<typename Type>
void editor::InspectProperty(Type& value)
{
	ImGui::PushID("<unknown>");

	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isContainer = refl::trait::is_container<Type>::value;
	constexpr bool isOverloaded = HasOverload<Type&>();
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	constexpr bool isVariant = core::IsVariant<Type>::value;

	if constexpr (isOverloaded)
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text("<unknown>");

		ImGui::TableSetColumnIndex(1);
		editor::TypeOverload(value);
	}
	else if constexpr (isClass && isReflectable)
	{
		editor::InspectClass(value);
	}
	else if constexpr (isVariant)
	{
		std::visit([&](auto& subValue)
		{
			editor::InspectVariant(value, subValue);
		}, value);
	}
	else if constexpr (isContainer)
	{
		// #note: we only reflect the container content type which is done internally
		editor::InspectContainer(value);
	}
	else
	{
		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text("<unknown>");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Unsupported Type!");
	}

	ImGui::PopID();
}

template<typename Type>
void editor::InspectClass(Type& value)
{
	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader("<unknown>");

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Members", refl::reflect<Type>().members.size);

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		editor::InspectType(value);

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}
}