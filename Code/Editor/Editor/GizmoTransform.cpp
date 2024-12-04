#include "EditorPCH.h"
#include "Editor/GizmoTransform.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EntityEditor.h"
#include "Editor/SettingsComponents.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/Screen.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/TransformComponent.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace
{
	enum class ETransformOper
	{
		Translate,
		Rotate,
		Scale,
	};

	enum class ETransformType
	{
		Physics,
		Transform,
	};

	ETransformOper s_TransformOper = ETransformOper::Translate;
	ETransformType s_TransformType = ETransformType::Transform;

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
}

void editor::GizmoTransform::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_FloorGrid;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::InputComponent>>())
	{
		const auto& input = world.ReadComponent<eng::InputComponent>(entity);
		if (input.IsKeyPressed(input::EKeyboard::F1))
			s_TransformType = ETransformType::Transform;
		if (input.IsKeyPressed(input::EKeyboard::F2))
			s_TransformType = ETransformType::Physics;

		if (input.IsKeyPressed(input::EKeyboard::Num_1))
			s_TransformOper = ETransformOper::Translate;
		if (input.IsKeyPressed(input::EKeyboard::Num_2))
			s_TransformOper = ETransformOper::Rotate;
		if (input.IsKeyPressed(input::EKeyboard::Num_3))
			s_TransformOper = ETransformOper::Scale;
	}

	ImGuiWindowClass windowClass;
	windowClass.ViewportFlagsOverrideClear |= ImGuiViewportFlags_NoAutoMerge;
	ImGui::SetNextWindowClass(&windowClass);

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	if (ImGui::Begin("##GizmoTransform", nullptr, s_Flags))
	{
		ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
		switch (s_TransformOper)
		{
		case ETransformOper::Translate:
			operation = ImGuizmo::TRANSLATE;
			break;
		case ETransformOper::Rotate:
			operation = ImGuizmo::ROTATE;
			break;
		case ETransformOper::Scale:
			operation = ImGuizmo::SCALE;
			break;
		}

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

			int32 index = 0;
			for (const ecs::Entity windowEntity : world.Query<ecs::query::Include<editor::EntityWindowComponent>>())
			{
				const auto& window = world.WriteComponent<editor::EntityWindowComponent>(windowEntity);
				if (window.m_Selected.IsUnassigned())
					continue;
				if (!world.IsAlive(window.m_Selected))
					continue;

				if (s_TransformType == ETransformType::Transform && world.HasComponent<eng::TransformComponent>(window.m_Selected))
				{
					auto& transform = world.WriteComponent<eng::TransformComponent>(window.m_Selected);

					Matrix4x4 objectTran = transform.ToTransform();

					ImGuizmo::SetID(index++);
					ImGuizmo::Manipulate(
						cameraView.m_Data[0],
						cameraProj.m_Data[0],
						operation,
						ImGuizmo::WORLD,
						objectTran.m_Data[0]);

					ImGuizmo::DecomposeMatrixToComponents(
						objectTran.m_Data[0],
						&transform.m_Translate.x,
						&transform.m_Rotate.m_Pitch,
						&transform.m_Scale.x);
				}

				if (s_TransformType == ETransformType::Physics && world.HasComponent<eng::PhysicsComponent>(window.m_Selected))
				{
					const bool isScale = s_TransformOper == ETransformOper::Scale;
					if (isScale)
						operation = ImGuizmo::BOUNDS;

					auto& physics = world.WriteComponent<eng::PhysicsComponent>(window.m_Selected);
					for (eng::Shape& shape : physics.m_Shapes)
					{
						Matrix4x4 parentTran = Matrix4x4::Identity;
						if (world.HasComponent<eng::TransformComponent>(window.m_Selected))
						{
							const auto& transform = world.ReadComponent<eng::TransformComponent>(window.m_Selected);
							parentTran = transform.ToTransform();
						}

						if (std::holds_alternative<eng::ShapeBox>(shape))
						{
							eng::ShapeBox& shapeBox = std::get<eng::ShapeBox>(shape);

							Matrix4x4 objectTran = Matrix4x4::FromTransform(
								shapeBox.m_Translate,
								shapeBox.m_Rotate,
								shapeBox.m_Extents);

							float bounds[6] = { -1.f, -1.f, -1.f, +1.f, +1.f, +1.f };

							objectTran *= parentTran;
							ImGuizmo::SetID(index++);
							ImGuizmo::Manipulate(
								cameraView.m_Data[0],
								cameraProj.m_Data[0],
								operation,
								ImGuizmo::WORLD,
								objectTran.m_Data[0],
								nullptr,
								nullptr,
								isScale ? bounds : nullptr);
							objectTran *= parentTran.Inversed();

							ImGuizmo::DecomposeMatrixToComponents(
								objectTran.m_Data[0],
								&shapeBox.m_Translate.x,
								&shapeBox.m_Rotate.m_Pitch,
								&shapeBox.m_Extents.x);
						}
					}
				}
			}
		}
		ImGui::End();
	}
}