#include "CraftingPCH.h"
#include "DebugCrafting/CraftingWindowSystem.h"

#include "Core/String.h"
#include "DebugCrafting/CraftingWindowComponent.h"
#include "DebugCrafting/CraftingWindowEvent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Math/Vector.h"
#include "SharedCrafting/CraftingIngredientTable.h"
#include "SharedCrafting/CraftingRecipeTable.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void debug::crafting::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 350.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::crafting::WindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();

		auto& window = world.AddComponent<debug::crafting::WindowComponent>(windowEntity);
		window.m_Label = ToLabel("Crafting Debugger", identifier);
		window.m_Identifier = identifier;

		const auto& ingredients = world.ReadResource<::crafting::IngredientTable>();
		for (const auto& [guid, ingredient] : ingredients.GetObjects())
		{
			window.m_Ingredients.Append(guid);
		}
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<debug::crafting::WindowComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<debug::crafting::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<debug::crafting::WindowComponent>>())
	{
		const auto& ingredients = world.ReadResource<::crafting::IngredientTable>();
		auto& window = windowView.WriteRequired<debug::crafting::WindowComponent>();

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			constexpr int32 s_Columns = 5;
			constexpr int32 s_Rows = 5;
			constexpr ImVec2 s_ItemSize = ImVec2(50.f, 50.f);
			constexpr ImVec2 s_TableSize = ImVec2(s_Columns * s_ItemSize.x + 10.f, s_Rows * s_ItemSize.y + 10.f);
			struct Payload { ecs::Entity m_Storage; ecs::Entity m_Member; };

			for (int32 i = 0; i < s_Columns * s_Rows; ++i)
			{
				ImGui::PushID(i);
				if (i % s_Rows != 0)
					ImGui::SameLine();

				if (i < window.m_Ingredients.GetCount())
				{
					const str::Guid& guid = window.m_Ingredients[i];
					const ::crafting::Ingredient& ingredient = ingredients.GetObject(guid);

					const str::String& label = ingredient.m_Name;
					ImGui::Button(label.c_str(), s_ItemSize);
				}
				else
				{
					ImGui::Button("", s_ItemSize);
				}

				ImGui::PopID();
			}

			ImGui::End();
		}

		if (!isWindowOpen)
			world.DestroyEntity(windowView);
	}
}