#include "EditorPCH.h"
#include "Editor/InputSystem.h"

#include "Core/EnumHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "GameDebug/MenuBarComponents.h"
#include "Input/Key.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void editor::InputSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const editor::InputWindowRequestComponent>>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Input Editor");

		auto& window = world.AddComponent<editor::InputWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Input Editor##input", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::InputWindowComponent>>())
	{
		auto& window = world.ReadComponent<editor::InputWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::InputWindowComponent>>())
	{
		auto& window = world.WriteComponent<editor::InputWindowComponent>(windowEntity);

		bool isOpen = true;
		bool isClient = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			const auto& inputManager = world.ReadResource<eng::InputManager>();
			for (const str::Name& name : inputManager.GetLayers())
			{
				ImGui::Text("%s", name.ToChar());
				ImGui::Indent();

				const input::Layer& layer = inputManager.GetLayer(name);
				if (layer.m_Consume.Has(input::Gamepad))
					ImGui::Text("Consume: All Gamepad");
				if (layer.m_Consume.Has(input::Keyboard))
					ImGui::Text("Consume: All Keyboard");
				if (layer.m_Consume.Has(input::Mouse))
					ImGui::Text("Consume: All Mouse");

				ImGui::Indent();
				for (const input::Binding& binding : layer.m_Bindings)
				{
					ImGui::Text("%s: ", binding.m_Name.ToChar());
					ImGui::SameLine();

					const str::String primary = str::String(EnumToString(binding.m_Primary));
					const str::String modifier = str::String(EnumToString(binding.m_Modifier));
					ImGui::Text("%s + %s", primary.c_str(), modifier.c_str());
					if (binding.m_Consume)
					{
						ImGui::SameLine();
						ImGui::Text(" - Consume");
					}
				}
				ImGui::Unindent();

				ImGui::Unindent();
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}
