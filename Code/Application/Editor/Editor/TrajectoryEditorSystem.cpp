#include "EditorPCH.h"
#include "Editor/TrajectoryEditorSystem.h"

#include "Core/Algorithms.h"
#include "Core/Trajectory.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/TrajectoryAsset.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileCreateResultComponent.h"
#include "GameDebug/MenuBarComponents.h"
#include "Math/Math.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	const str::Guid strStaticMesh = str::Guid::Create("d0284c563e6a49a29a8025ff2731a3de");

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	using World = editor::TrajectoryEditorSystem::World;

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					eng::TrajectoryAsset trajectory;
					trajectory.m_Points.Append(Vector2f(0.f, 0.f));
					trajectory.m_Points.Append(Vector2f(0.1f, 0.1f));
					trajectory.m_Points.Append(Vector2f(-0.15f, 0.15f));
					trajectory.m_Points.Append(Vector2f(0.3f, 0.3f));
					trajectory.m_Points.Append(Vector2f(-0.35f, 0.4f));
					trajectory.m_Points.Append(Vector2f(0.5f, 0.5f));
					trajectory.m_Points.Append(Vector2f(-0.2f, 0.6f));
					trajectory.m_Points.Append(Vector2f(0.1f, 0.7f));
					trajectory.m_Points.Append(Vector2f(0.f, 1.f));

					auto& windowComponent = world.WriteComponent<editor::TrajectoryWindowComponent>(entity);
					windowComponent.m_Asset = trajectory;
				}

				if (ImGui::MenuItem("Open"))
					world.AddComponent<editor::TrajectoryAssetOpenComponent>(entity);

				if (ImGui::MenuItem("Save"))
					world.AddComponent<editor::TrajectoryAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::TrajectoryWindowComponent>(entity);
		eng::TrajectoryAsset& trajectory = windowComponent.m_Asset;

		imgui::InputText("m_Guid", trajectory.m_Guid);
		imgui::InputText("m_Name", trajectory.m_Name);

		ImGui::Separator();

		if (ImGui::CollapsingHeader("m_Positions"))
		{
			Array<Vector2f>& values = trajectory.m_Points;

			ImGui::Indent();
			if (ImGui::Button("Add"))
				values.Append(Vector2f::Zero);
			ImGui::SameLine();
			if (ImGui::Button("Pop"))
				values.Pop();
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
				values.RemoveAll();
			ImGui::SameLine();
			if (ImGui::Button("Spawn"))
			{
				path::Points path;

				float distance = 0.f;
				for (int32 i = 0; i < values.GetCount(); ++i)
				{
					const Vector3f positionI = Vector3f(values[i].x, 0.f, values[i].y);
					if (i != 0)
					{
						const Vector3f positionJ = Vector3f(values[i - 1].x, 0.f, values[i-1].y);
						distance += math::Distance(positionJ, positionI);
					}

					path.m_Positions.Append(positionI);
					path.m_Distances.Append(distance);
				}

				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<projectile::CreateRequestComponent>(entity);
				requestComponent.m_Lifetime.m_Timeout = 5.f;
				requestComponent.m_Trajectory.m_Trajectory = path::Trajectory(path);
				requestComponent.m_Trajectory.m_Scale = 1000.f;
				requestComponent.m_Transform.m_Scale = Vector3f(0.1f);
				requestComponent.m_Velocity.m_Initial = 1000.f;
				requestComponent.m_Visual.m_StaticMesh = strStaticMesh;
			}

			for (auto&& [i, position] : enumerate::Forward(values))
			{
				ImGui::PushID(i);
				ImGui::DragFloat2("", &position.x);
				ImGui::PopID();
			}
		}
	}

	void DrawPlotter(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::TrajectoryWindowComponent>(entity);
		eng::TrajectoryAsset& trajectory = windowComponent.m_Asset;

		Array<Vector2f>& values = trajectory.m_Points;
		if (ImGui::Button("Normalize"))
		{
			Vector2f range_min = Vector2f(+FLT_MAX);
			Vector2f range_max = Vector2f(-FLT_MAX);
			for (const Vector2f& value : values)
			{
				range_min = math::Min(range_min, value);
				range_max = math::Max(range_max, value);
			}

			for (Vector2f& value : values)
			{
				value.x = math::Remap(value.x, range_min.x, range_max.x, 0.f, 1.f);
				value.y = math::Remap(value.y, range_min.y, range_max.y, 0.f, 1.f);
			}
		}

		const ImGuiPlotFlags flags = ImGuiPlotFlags_Grid | ImGuiPlotFlags_TextX | ImGuiPlotFlags_TextY;
		const Vector2f size = ImGui::GetContentRegionAvail();
		imgui::PlotLines("##plotter", values.GetData(), values.GetCount(), size, flags);
	}

	void DrawPopupOpen(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TrajectoryAssetOpenComponent>(entity))
		{
			world.RemoveComponent<editor::TrajectoryAssetOpenComponent>(entity);
		}
	};

	void DrawPopupSave(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (world.HasComponent<editor::TrajectoryAssetSaveComponent>(entity))
		{
			auto& windowComponent = world.WriteComponent<editor::TrajectoryWindowComponent>(entity);
			const str::Name& name = windowComponent.m_Asset.m_Name;

			eng::SaveFileSettings settings;
			settings.m_Title = "Save Trajectory";
			settings.m_Filters = { "Assets (*.asset)", "*.asset" };
			settings.m_Path = str::GetPath(str::EPath::Assets);
			settings.m_Path += name;

			const str::Path filepath = eng::SaveFileDialog(settings);
			if (!filepath.IsEmpty())
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveAsset(windowComponent.m_Asset, filepath);
			}

			world.RemoveComponent<editor::TrajectoryAssetSaveComponent>(entity);
		}
	};
}

void editor::TrajectoryEditorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const auto& request : world.Events<editor::TrajectoryWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Trajectory Editor");

		auto& window = world.AddComponent<editor::TrajectoryWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Trajectory Editor", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##trajectory", identifier);
		window.m_PlottingLabel = ToLabel("Plotter##trajectory", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const editor::TrajectoryWindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::TrajectoryWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::TrajectoryWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::TrajectoryWindowComponent>(windowEntity);

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID inspectorId, plottingId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.7f, &plottingId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_PlottingLabel.c_str(), plottingId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_PlottingLabel.c_str()))
			DrawPlotter(world, windowEntity);
		ImGui::End();

		DrawPopupOpen(world, windowEntity);
		DrawPopupSave(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::CreateResultComponent>>())
		world.DestroyEntity(entity);
}