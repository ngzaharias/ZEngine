#include "EditorPCH.h"
#include "Editor/TemplateEditor.h"

#include <Core/Address.h>
#include <Core/Parse.h>

#include <Engine/AssetManager.h>
#include <Engine/ExampleAsset.h>
#include <Engine/TomlHelpers.h>
#include <Engine/TomlTypes.h>

#include <iostream>
#include <imgui/Custom.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "Editor/Transaction.h"

// #todo: custom widgets per property
// #todo: types:
//	- set
//	- variant
// #todo: change -> transaction -> record -> historian
// #todo: change -> diff -> transaction
// #todo: error on duplicate in set/map
// #todo: remove single element array/set/map
// #todo: re-arrange elements array/set/map

namespace
{
	static str::String s_String = "";

	const str::Name strData = NAME("Data");
	const str::Name strDefault = NAME("Default");
	const str::Name strName = NAME("Name");
	const str::Name strGuid = NAME("Guid");
	const str::Name strValue = NAME("Value");
	const str::Name strVersion = NAME("Version");

	void DiffApply(const toml::Value& input, toml::Value& output)
	{
		for (auto&& [key, value] : input.as_table())
		{
			if (value.is_table())
			{
				DiffApply(value, output[key]);
			}
			else
			{
				output[key] = value;
			}
		}
	}

	bool DiffUtil(const toml::Value& inputA, const toml::Value& inputB, toml::Value& output)
	{
		if (inputA.is_table())
		{
			bool hasDifference = false;
			for (auto&& [key, valueA] : inputA.as_table())
			{
				const toml::Value& valueB = inputB.at(key);

				toml::Value transaction;
				if (DiffUtil(valueA, valueB, transaction))
				{
					hasDifference = true;
					output[key] = transaction;
				}
			}
			return hasDifference;
		}
		else if (inputA != inputB)
		{
			output = inputB;
			return true;
		}
		return false;
	}

	void InspectObject(const toml::key& key, toml::Value& value);

	void InspectArray(const toml::key& key, toml::Value& value, const toml::Value& default)
	{
		if (default.is_uninitialized())
			return;

		ImGui::TableSetColumnIndex(0);
		const bool isExpanded = imgui::InspectHeader(key.c_str());

		auto& data = value.as_array();
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%d Elements \t", data.size());
		ImGui::SameLine(ImGui::GetColumnWidth() - 40.f);
		if (ImGui::Button(" + "))
			data.push_back(default);
		ImGui::SameLine();
		if (ImGui::Button(" x "))
			data.clear();
		ImGui::TableNextRow();

		if (isExpanded)
		{
			ImGui::TableSetColumnIndex(0);
			ImGui::Indent();

			for (auto&& [i, child] : enumerate::Forward(data))
			{
				s_String = std::to_string(i++);
				InspectObject(s_String.c_str(), child);
			}

			ImGui::TableSetColumnIndex(0);
			ImGui::Unindent();
		}
	}

	void InspectBoolean(const toml::key& key, toml::Value& value)
	{
		bool data = value.as_boolean();

		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(key.c_str());

		s_String = "##" + key;
		ImGui::TableSetColumnIndex(1);
		if (ImGui::Checkbox(s_String.c_str(), &data))
			value = data;
	}

	void InspectFloat(const toml::key& key, toml::Value& value)
	{
		float data = static_cast<float>(value.as_floating());

		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(key.c_str());

		s_String = "##" + key;
		ImGui::TableSetColumnIndex(1);
		if (ImGui::DragFloat(s_String.c_str(), &data))
			value = data;
	}

	void InspectInteger(const toml::key& key, toml::Value& value)
	{
		int32 data = static_cast<int32>(value.as_integer());

		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(key.c_str());

		s_String = "##" + key;
		ImGui::TableSetColumnIndex(1);
		if (ImGui::DragInt(s_String.c_str(), &data))
			value = data;
	}

	void InspectString(const toml::key& key, toml::Value& value)
	{
		str::String data = value.as_string();

		ImGui::TableSetColumnIndex(0);
		imgui::Bullet();
		ImGui::Text(key.c_str());

		s_String = "##" + key;
		ImGui::TableSetColumnIndex(1);
		if (ImGui::InputText(s_String.c_str(), &data))
			value = data;
	}

	void InspectObject(const toml::key& key, toml::Value& value)
	{
		ImGui::PushID(key.c_str());

		// #note: if the type is implicit then we can just use our current value
		toml::Value& data = toml::find_or(value, strValue.ToChar(), value);

		// #edge-case: all containers are tread as arrays
		if (data.is_array())
		{
			toml::Value default = toml::find_or(value, strDefault.ToChar(), {});
			InspectArray(key, data, default);
			ImGui::TableNextRow();
		}
		else if (data.is_boolean())
		{
			InspectBoolean(key, data);
			ImGui::TableNextRow();
		}
		else if (data.is_floating())
		{
			InspectFloat(key, data);
			ImGui::TableNextRow();
		}
		else if (data.is_integer())
		{
			InspectInteger(key, data);
			ImGui::TableNextRow();
		}
		else if (data.is_string())
		{
			InspectString(key, data);
			ImGui::TableNextRow();
		}
		// #edge-case: all objects are treated as tables
		else if (data.is_table())
		{
			ImGui::TableSetColumnIndex(0);
			const bool isExpanded = imgui::InspectHeader(key.c_str());
			ImGui::TableNextRow();

			if (isExpanded)
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Indent();

				for (auto&& [childK, childV] : data.as_table())
					InspectObject(childK, childV);

				ImGui::TableSetColumnIndex(0);
				ImGui::Unindent();
			}
		} 

		ImGui::PopID();
	}
}

editor::TemplateEditor::TemplateEditor(asset::Manager& assetManager)
	: m_AssetManager(assetManager)
{
}

editor::TemplateEditor::~TemplateEditor()
{
}

void editor::TemplateEditor::Initialise()
{
	PROFILE_FUNCTION();
}

void editor::TemplateEditor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_IsVisible)
		return;

	Render();
}

void editor::TemplateEditor::Render()
{
	if (ImGui::Begin("TemplateEditor", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		Render_MenuBar();
		Render_Selection();
	}
	ImGui::End();
}

void editor::TemplateEditor::Render_MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// #todo: move to main menu bar
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				const str::String guid = toml::find_or<std::string>(m_DocumentNew, strGuid.ToChar(), "<ERROR>");
				m_AssetManager.SaveAsset<asset::ExampleAsset>(GUID(guid));
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void editor::TemplateEditor::Render_Selection()
{
	if (m_DocumentNew.is_uninitialized())
		return;

	const str::String name = toml::find_or<std::string>(m_DocumentNew, strName.ToChar(), "<ERROR>");
	const str::String guid = toml::find_or<std::string>(m_DocumentNew, strGuid.ToChar(), "<ERROR>");

	if (ImGui::BeginChild("Selection"))
	{
		if (imgui::InspectHeader(name.c_str()))
		{
			m_DocumentOld = m_DocumentNew;

			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.f);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1.f, 1.f });
			ImGui::BeginTable("Data", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(-FLT_MIN);

			InspectString(strName.ToChar(), toml::find(m_DocumentNew, strName.ToChar()));
			ImGui::TableNextRow();

			InspectString(strGuid.ToChar(), toml::find(m_DocumentNew, strGuid.ToChar()));
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Separator();
			ImGui::TableSetColumnIndex(1);
			ImGui::Separator();
			ImGui::TableNextRow();

			for (auto&& [key, value] : m_DocumentNew.as_table())
			{
				if (value.is_table())
					InspectObject(key, value);
			}

			ImGui::EndTable();
			ImGui::PopStyleVar(2);
		}
	}
	ImGui::EndChild();

	if (m_DocumentOld != m_DocumentNew)
	{
		m_AssetManager.UpdateAsset<asset::ExampleAsset>(GUID(guid), m_DocumentNew);
	}
}