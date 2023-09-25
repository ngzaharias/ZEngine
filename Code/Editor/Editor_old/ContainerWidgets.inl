#pragma once

#include <Core/Address.h>
#include <Core/VariantHelpers.h>

#include <Engine/AttributeHelpers.h>
#include <Engine/TypeHelpers.h>

#include <imgui/Custom.h>
#include <imgui/Helpers.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Editor/ContainerWidgets.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/TrivialWidgets.h"

template<class Container>
void editor::InspectContainer(Container& container)
{
	ImGui::TableSetColumnIndex(0);
	imgui::Bullet();
	ImGui::Text("Unknown");

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("Unsupported Container Type!");
}

template<typename Type>
void editor::InspectContainer(Array<Type>& container)
{
	//struct None { };
	//struct DragDrop { int32 source, target; };
	//struct Insert { int32 index; };
	//struct RemoveAll { };

	//using Command = Variant<None, DragDrop, Insert, RemoveAll>;
	//using Iterator = typename Array<Type>::iterator;

	//if constexpr (!refl::trait::is_reflectable<Type>::value)
	//	return;

	//Command command = None();

	//ImGui::TableSetColumnIndex(0);
	//const bool isExpanded = imgui::InspectHeader("<unknown>");

	//ImGui::TableSetColumnIndex(1);
	//ImGui::Text("%d Elements", container.size());
	//ImGui::SameLine(ImGui::GetColumnWidth() - 50.f);
	//if (ImGui::Button("+"))
	//	command = Insert{ static_cast<int32>(container.size()) };
	//ImGui::SameLine();
	//if (ImGui::Button("x"))
	//	command = RemoveAll();

	//if (isExpanded)
	//{
	//	ImGui::TableNextRow();
	//	ImGui::TableSetColumnIndex(0);
	//	ImGui::Indent();

	//	Iterator itr = container.begin();
	//	Iterator end = container.end();
	//	for (int32 i = 0; itr != end; ++itr, ++i)
	//	{
	//		editor::InspectProperty(itr);
	//		ImGui::TableNextRow();
	//	}

	//	ImGui::TableSetColumnIndex(0);
	//	ImGui::Unindent();
	//}

	//std::visit(core::Overload
	//	{
	//		[&](auto& arg) {},
	//		[&](DragDrop& args)
	//		{
	//			const bool isBefore = args.source < args.target;
	//			Iterator source = std::next(container.begin(), args.source);
	//			Iterator target = std::next(container.begin(), isBefore ? args.target + 1 : args.target);
	//			container.insert(target, *source);

	//			// iterators were invalidated
	//			source = std::next(container.begin(), !isBefore ? args.source + 1 : args.source);
	//			container.erase(source);
	//		},
	//		[&](Insert& args)
	//		{
	//			Iterator itr = container.begin() + args.index;
	//			container.insert(itr, Type());
	//		},
	//		[&](RemoveAll& args)
	//		{
	//			container.clear();
	//		},
	//	}, command);
}

template<class Key, class Value>
void editor::InspectContainer(Map<Key, Value>& container)
{
	struct None { };
	struct Insert { Key key; };
	struct RemoveAll { };
	struct Rename { int32 index; Key key; };

	using Iterator = typename Map<Key, Value>::iterator;
	using Command = Variant<None, Insert, RemoveAll, Rename>;

	if constexpr (!refl::trait::is_reflectable<Key>::value || !refl::trait::is_reflectable<Value>::value)
		return;

	Command command = None();

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader("<unknown>");

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Elements", container.size());
	ImGui::SameLine(ImGui::GetColumnWidth() - 50.f);
	if (ImGui::Button("+"))
		command = Insert{ Key() };
	ImGui::SameLine();
	if (ImGui::Button("x"))
		command = RemoveAll();

	if (isExpanded)
	{
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		constexpr bool isKeyInlined = !std::is_class<Key>::value;
		constexpr bool isValInlined = !std::is_class<Value>::value;

		Iterator itr = container.begin();
		Iterator end = container.end();
		for (int32 i = 0, id = 0; itr != end; ++itr, ++i)
		{
			Key key = itr->first;
			Value& value = itr->second;

			// key
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(0);
				imgui::Bullet();
				ImGui::SetNextItemWidth(-1);
				editor::InspectType(key);
			}
			if (isKeyInlined && !isValInlined)
			{
				editor::InspectProperty(key);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && isValInlined)
			{
				editor::InspectProperty(key);
				ImGui::TableNextRow();
			}
			if (!isKeyInlined && !isValInlined)
			{
				editor::InspectProperty(key);
				ImGui::TableNextRow();
			}
			ImGui::PopID();

			// value
			ImGui::PushID(++id);
			if (isKeyInlined && isValInlined)
			{
				ImGui::TableSetColumnIndex(1);
				editor::InspectType(value);
			}
			if (isKeyInlined && !isValInlined)
				editor::InspectProperty(value);
			if (!isKeyInlined && isValInlined)
				editor::InspectProperty(value);
			if (!isKeyInlined && !isValInlined)
				editor::InspectProperty(value);
			ImGui::PopID();

			if (key != itr->first && ImGui::IsItemDeactivatedAfterEdit())
				command = Rename{ i, key };

			ImGui::TableNextRow();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}

	std::visit(core::Overload
		{
			[&](const None& arg) {},
			[&](const Insert& arg)
			{
				container.try_emplace(arg.key);
			},
			[&](const RemoveAll& arg) { container.clear(); },
			[&](const Rename& arg)
			{
				if (container.count(arg.key) == 0)
				{
					Iterator itr = std::next(container.begin(), arg.index);
					auto node = container.extract(itr);
					node.key() = arg.key;
					container.insert(std::move(node));
				}
			}
		}, command);
}

template<typename Type>
void editor::InspectContainer(Set<Type>& container)
{
	using Iterator = typename Set<Type>::iterator;

	if constexpr (!refl::trait::is_reflectable<Type>::value)
		return;

	ImGui::TableSetColumnIndex(0);
	const bool isExpanded = imgui::InspectHeader("<unknown>");

	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%d Elements", container.size());

	if (isExpanded)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Indent();

		Iterator itr = container.begin();
		Iterator end = container.end();
		for (int32 i = 0; itr != end; ++itr, ++i)
		{
			editor::InspectProperty(itr);
			ImGui::TableNextRow();
		}

		ImGui::TableSetColumnIndex(0);
		ImGui::Unindent();
	}
}