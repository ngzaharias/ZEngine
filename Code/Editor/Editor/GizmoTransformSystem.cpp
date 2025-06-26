#include "EditorPCH.h"
#include "Editor/GizmoTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EntitySelectComponent.h"
#include "Editor/SettingsComponents.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imguizmo/ImGuizmo.h"

namespace
{
	const str::Name strInput = str::Name::Create("GizmoTransform");
	const str::Name strPhysics = str::Name::Create("GizmoTransform_Physics");
	const str::Name strRotate = str::Name::Create("GizmoTransform_Rotate");
	const str::Name strScale = str::Name::Create("GizmoTransform_Scale");
	const str::Name strTransform = str::Name::Create("GizmoTransform_Transform");
	const str::Name strTranslate = str::Name::Create("GizmoTransform_Translate");

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
void editor::gizmo::TransformSystem::Initialise(World& world)
{
	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::EditorUI;
	layer.m_Bindings.Emplace(strTransform, input::EKey::F1);
	layer.m_Bindings.Emplace(strPhysics, input::EKey::F2);
	layer.m_Bindings.Emplace(strTranslate, input::EKey::Num_1);
	layer.m_Bindings.Emplace(strRotate, input::EKey::Num_2);
	layer.m_Bindings.Emplace(strScale, input::EKey::Num_3);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, layer);
}

void editor::gizmo::TransformSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void editor::gizmo::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalComponent>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_Transform;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (!debugSettings.m_IsEditorModeEnabled)
		return;

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strTransform))
			s_TransformType = ETransformType::Transform;
		if (input.IsPressed(strPhysics))
			s_TransformType = ETransformType::Physics;

		if (input.IsPressed(strTranslate))
			s_TransformOper = ETransformOper::Translate;
		if (input.IsPressed(strRotate))
			s_TransformOper = ETransformOper::Rotate;
		if (input.IsPressed(strScale))
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

		const Vector2u& resolution = window->GetResolution();
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::EditorComponent, const eng::camera::ProjectionComponent>>())
		{
			const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Matrix4x4 cameraProj = eng::camera::GetProjection(resolution, cameraProjection.m_Projection);
			const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

			const bool isOrthographic = std::holds_alternative<eng::camera::Orthographic>(cameraProjection.m_Projection);
			ImGuizmo::SetOrthographic(isOrthographic);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

			const auto& selectComponent = world.ReadSingleton<editor::EntitySelectComponent>();
			const ecs::Entity selected = selectComponent.m_Entity;
			if (selected.IsUnassigned())
				continue;
			if (!world.IsAlive(selected))
				continue;

			if (s_TransformType == ETransformType::Transform && world.HasComponent<eng::TransformComponent>(selected))
			{
				auto& transform = world.WriteComponent<eng::TransformComponent>(selected);

				Matrix4x4 objectTran = transform.ToTransform();

				ImGuizmo::SetID(0);
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

			if (s_TransformType == ETransformType::Physics && world.HasComponent<eng::PhysicsComponent>(selected))
			{
				const bool isScale = s_TransformOper == ETransformOper::Scale;
				if (isScale)
					operation = ImGuizmo::BOUNDS;

				auto& physics = world.WriteComponent<eng::PhysicsComponent>(selected);
				for (eng::Shape& shape : physics.m_Shapes)
				{
					Matrix4x4 parentTran = Matrix4x4::Identity;
					if (world.HasComponent<eng::TransformComponent>(selected))
					{
						const auto& transform = world.ReadComponent<eng::TransformComponent>(selected);
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
						ImGuizmo::SetID(0);
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
		ImGui::End();
	}
}