#include "EditorPCH.h"
#include "Editor/GizmoTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Editor/GizmoTransformSingleton.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "EntityEditor/EntityEditorSelectSingleton.h"
#include "Math/Matrix.h"
#include "Settings/SettingsEditorComponent.h"

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

	using World = editor::gizmo::TransformSystem::World;
	void ProcessInput(World& world)
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strTransform))
		{
			auto& gizmo = world.WriteSingleton<editor::gizmo::TransformSingleton>();
			gizmo.m_TransformType = editor::gizmo::ETransformType::Transform;
		}
		if (input.IsPressed(strPhysics))
		{
			auto& gizmo = world.WriteSingleton<editor::gizmo::TransformSingleton>();
			gizmo.m_TransformType = editor::gizmo::ETransformType::Physics;
		}

		if (input.IsPressed(strTranslate))
		{
			auto& gizmo = world.WriteSingleton<editor::gizmo::TransformSingleton>();
			gizmo.m_TransformOper = editor::gizmo::ETransformOper::Translate;
		}
		if (input.IsPressed(strRotate))
		{
			auto& gizmo = world.WriteSingleton<editor::gizmo::TransformSingleton>();
			gizmo.m_TransformOper = editor::gizmo::ETransformOper::Rotate;
		}
		if (input.IsPressed(strScale))
		{
			auto& gizmo = world.WriteSingleton<editor::gizmo::TransformSingleton>();
			gizmo.m_TransformOper = editor::gizmo::ETransformOper::Scale;
		}
	}

	void ProcessGizmo(World& world)
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

		const auto& windowManager = world.ReadResource<eng::WindowManager>();
		const eng::Window* window = windowManager.GetWindow(0);
		if (!window)
			return;

		const auto& gizmo = world.ReadSingleton<editor::gizmo::TransformSingleton>();

		ImGuiWindowClass windowClass;
		windowClass.ViewportFlagsOverrideClear |= ImGuiViewportFlags_NoAutoMerge;
		ImGui::SetNextWindowClass(&windowClass);

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (ImGui::Begin("##GizmoTransform", nullptr, s_Flags))
		{
			ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
			switch (gizmo.m_TransformOper)
			{
			case editor::gizmo::ETransformOper::Translate:
				operation = ImGuizmo::TRANSLATE;
				break;
			case editor::gizmo::ETransformOper::Rotate:
				operation = ImGuizmo::ROTATE;
				break;
			case editor::gizmo::ETransformOper::Scale:
				operation = ImGuizmo::SCALE;
				break;
			}

			const Vector2u& windowSize = window->GetSize();
			using Query = ecs::query
				::Include<
				const eng::ActiveComponent,
				const eng::CameraComponent,
				const eng::TransformComponent>;
			for (auto&& view : world.Query<Query>())
			{
				const auto& cameraProjection = view.ReadRequired<eng::CameraComponent>();
				const auto& cameraTransform = view.ReadRequired<eng::TransformComponent>();

				const Matrix4x4 cameraProj = eng::GetProjection(cameraProjection.m_Projection, windowSize);
				const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

				const bool isOrthographic = std::holds_alternative<eng::Orthographic>(cameraProjection.m_Projection);
				ImGuizmo::SetOrthographic(isOrthographic);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

				const auto& selectComponent = world.ReadSingleton<editor::entity::SelectSingleton>();
				const ecs::Entity selected = selectComponent.m_Entity;
				if (selected.IsUnassigned())
					continue;
				if (!world.IsAlive(selected))
					continue;

				if (gizmo.m_TransformType == editor::gizmo::ETransformType::Transform && world.HasComponent<eng::TransformComponent>(selected))
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

				if (gizmo.m_TransformType == editor::gizmo::ETransformType::Physics && world.HasComponent<eng::PhysicsComponent>(selected))
				{
					const bool isScale = gizmo.m_TransformOper == editor::gizmo::ETransformOper::Scale;
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
}
void editor::gizmo::TransformSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

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
	PROFILE_FUNCTION();

	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void editor::gizmo::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& singleton = world.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& gizmos = singleton.m_Gizmos;
	const auto& settings = gizmos.m_Transform;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	if (!world.HasAny<ecs::query::Include<::settings::EditorComponent>>())
		return;

	ProcessInput(world);
	ProcessGizmo(world);
}