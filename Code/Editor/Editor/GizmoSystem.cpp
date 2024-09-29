#include "EditorPCH.h"
#include "Editor/GizmoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EntityEditor.h"
#include "Editor/SettingsComponents.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

void editor::GizmoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = settings.m_Gizmos;
	if (!gizmos.m_IsEnabled)
		return;

	// render coordinate lines
	if (gizmos.m_ShowCoordinates)
	{
		constexpr float s_Distance = 100000.f;
		constexpr Vector4f s_AxisX = Vector4f(1.f, 0.f, 0.f, 1.f);
		constexpr Vector4f s_AxisY = Vector4f(0.f, 1.f, 0.f, 1.f);
		constexpr Vector4f s_AxisZ = Vector4f(0.f, 0.f, 1.f, 1.f);

		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		linesComponent.AddLine(Vector3f::AxisX * -s_Distance, Vector3f::AxisX * s_Distance, s_AxisX);
		linesComponent.AddLine(Vector3f::AxisY * -s_Distance, Vector3f::AxisY * s_Distance, s_AxisY);
		linesComponent.AddLine(Vector3f::AxisZ * -s_Distance, Vector3f::AxisZ * s_Distance, s_AxisZ);
	}

	// imgui gizmos
	{
		constexpr auto s_Flags =
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoMouseInputs |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoNavInputs |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoTitleBar;

		ImGuiWindowClass windowClass;
		windowClass.ViewportFlagsOverrideClear |= ImGuiViewportFlags_NoAutoMerge;
		ImGui::SetNextWindowClass(&windowClass);

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (ImGui::Begin("##gizmo", nullptr, s_Flags))
		{
			if (gizmos.m_ShowTransforms)
			{
				for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent>>())
				{
					const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
					const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

					const Vector2u screenSize = Vector2u(
						static_cast<uint32>(Screen::width), 
						static_cast<uint32>(Screen::height));

					Matrix4x4 cameraProj = eng::camera::GetProjection(screenSize, cameraProjection.m_Projection);
					Matrix4x4 cameraView = cameraTransform.ToTransform();
					cameraView.Inverse();

					const bool isOrthographic = std::holds_alternative<eng::camera::Orthographic>(cameraProjection.m_Projection);
					ImGuizmo::SetOrthographic(isOrthographic);
					ImGuizmo::SetDrawlist();
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
					for (const ecs::Entity windowEntity : world.Query<ecs::query::Include<editor::EntityWindowComponent>>())
					{
						const auto& window = world.WriteComponent<editor::EntityWindowComponent>(windowEntity);
						if (window.m_Selected.IsUnassigned())
							continue;
						if (!world.IsAlive(window.m_Selected))
							continue;
						if (!world.HasComponent<eng::TransformComponent>(window.m_Selected))
							continue;

						auto& transform = world.WriteComponent<eng::TransformComponent>(window.m_Selected);

						Matrix4x4 objectTran = transform.ToTransform();

						ImGuizmo::SetID(window.m_Selected.GetIndex());
						ImGuizmo::Manipulate(
							cameraView.m_Data[0],
							cameraProj.m_Data[0],
							ImGuizmo::TRANSLATE,
							ImGuizmo::WORLD,
							objectTran.m_Data[0]);

						ImGuizmo::DecomposeMatrixToComponents(
							objectTran.m_Data[0],
							&transform.m_Translate.x,
							&transform.m_Rotate.m_Pitch,
							&transform.m_Scale.x);
					}
				}
			}
			ImGui::End();
		}
	}
}