#include "EditorPCH.h"
#include "Editor/OverlaySystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void editor::OverlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_Flags =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoFocusOnAppearing |
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

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	const Vector2f viewportPos = viewport->Pos;
	const Vector2f viewportSize = viewport->Size;

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowClass(&windowClass);
	ImGui::SetNextWindowPos(viewportPos + Vector2f(viewportSize.x * 0.5f - 32.f, 32.f));
	ImGui::SetNextWindowSize(Vector2f(80.f, 16.f));
	if (ImGui::Begin("Game Mode##editor", nullptr, s_Flags))
	{
		auto& settings = world.WriteSingleton<eng::settings::DebugComponent>();
		if (ImGui::Selectable("Game Mode", !settings.m_IsEditorModeEnabled))
			settings.m_IsEditorModeEnabled = !settings.m_IsEditorModeEnabled;
	}
	ImGui::End();

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowClass(&windowClass);
	ImGui::SetNextWindowPos(viewportPos + Vector2f(viewportSize.x - 128.f, 32.f));
	ImGui::SetNextWindowSize(Vector2f(100.f, 100.f));
	if (ImGui::Begin("Camera##editor", nullptr, s_Flags))
	{
		ecs::Entity cameraEntity = {};
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::EditorComponent>>())
			cameraEntity = entity;

		if (!cameraEntity.IsUnassigned())
		{
			Optional<eng::camera::Projection> projection = {};
			const auto& readCamera = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			if (std::holds_alternative<eng::camera::Orthographic>(readCamera.m_Projection))
			{
				if (ImGui::Button("Orthographic"))
					projection = eng::camera::Perspective{};
			}
			else
			{
				if (ImGui::Button("Perspective"))
					projection = eng::camera::Orthographic{};
			}

			Optional<Rotator> rotation = {};
			if (ImGui::Button("-X"))
				rotation = Rotator(0.f, -90.f, 0.f);
			ImGui::SameLine();
			if (ImGui::Button("+X"))
				rotation = Rotator(0.f, +90.f, 0.f);

			if (ImGui::Button("-Y"))
				rotation = Rotator(+90.f, 0.f, 0.f);
			ImGui::SameLine();
			if (ImGui::Button("+Y"))
				rotation = Rotator(-90.f, 0.f, 0.f);

			if (ImGui::Button("-Z"))
				rotation = Rotator(0.f, 180.f, 0.f);
			ImGui::SameLine();
			if (ImGui::Button("+Z"))
				rotation = Rotator(0.f, 0.f, 0.f);

			if (projection)
			{
				auto& writeCamera = world.WriteComponent<eng::camera::ProjectionComponent>(cameraEntity);
				writeCamera.m_Projection = *projection;
			}

			if (rotation)
			{
				auto& writeTransform = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				writeTransform.m_Rotate = *rotation;
			}
		}
	}
	ImGui::End();
}
