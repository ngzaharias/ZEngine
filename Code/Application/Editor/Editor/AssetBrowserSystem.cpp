#include "EditorPCH.h"
#include "Editor/AssetBrowserSystem.h"

#include "Core/SortHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "GameDebug/EditorAssetBrowserWindowRequest.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	using World = editor::AssetBrowserSystem::World;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	void Sort(Array<eng::AssetFile>& value)
	{
		std::sort(value.begin(), value.end(), 
			[](const eng::AssetFile& a, const eng::AssetFile& b) -> int
			{
				return a.m_Path < b.m_Path;
				//return sort::AlphaNumeric(a.m_Path, b.m_Path) < 0;
			});
	}

	void QSort(Array<eng::AssetFile>& value)
	{
		std::qsort(
			value.GetData(),
			value.GetCount(),
			sizeof(eng::AssetFile),
			[](const void* a, const void* b) -> int
			{
				const auto& arg1 = *static_cast<const eng::AssetFile*>(a);
				const auto& arg2 = *static_cast<const eng::AssetFile*>(b);
				return sort::AlphaNumeric(arg1.m_Path, arg2.m_Path);
			});
	}
}

void editor::AssetBrowserSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const auto& request : world.Events<editor::AssetBrowserWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Asset Browser");

		auto& window = world.AddComponent<editor::AssetBrowserWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Asset Browser", identifier);

		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		for (const auto& [guid, file] : assetManager.GetFileMap())
			window.m_Sorted.Append(file);
		Sort(window.m_Sorted);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::AssetBrowserWindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::AssetBrowserWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::AssetBrowserWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::AssetBrowserWindowComponent>(windowEntity);

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			str::String label;
			const auto& assetManager = world.ReadResource<eng::AssetManager>();
			for (const eng::AssetFile& file : windowComponent.m_Sorted)
			{
				ImGui::Selectable(file.m_Path.ToChar());
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("eng::AssetFile", &file, sizeof(eng::AssetFile));
					ImGui::EndDragDropSource();
				}
			}
		}
		ImGui::End();

		if (!isWindowOpen)
			world.DestroyEntity(windowEntity);
	}
}