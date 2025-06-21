#include "EditorPCH.h"
#include "Editor/CameraSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/LevelComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	const str::Guid strIconPlay = GUID("fad14b87898b4c13aae829c3b8756aec");
	const str::Guid strIconStop = GUID("7b71eda0a582471e9531ec36671ae413");
}

void editor::CameraSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (debugSettings.m_IsEditorModeEnabled && !world.HasAny<ecs::query::Include<const eng::camera::EditorComponent>>())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Editor Camera");
		world.AddComponent<eng::camera::EditorComponent>(entity);
		world.AddComponent<eng::camera::Move3DComponent>(entity);
		world.AddComponent<eng::camera::Pan3DComponent>(entity);
		world.AddComponent<eng::camera::ProjectionComponent>(entity);
		world.AddComponent<eng::camera::Zoom2DComponent>(entity);
		auto& transform = world.AddComponent<eng::TransformComponent>(entity);
		transform.m_Rotate = Rotator(+20.f, -45.f, 0.f);
		transform.m_Translate = Vector3f(+1000.f, +500.f, -1000.f);
	}

	if (!debugSettings.m_IsEditorModeEnabled && world.HasAny<ecs::query::Include<const eng::camera::EditorComponent>>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::camera::EditorComponent>>())
			world.DestroyEntity(entity);
	}

	constexpr ImGuiWindowFlags s_Flags =
		ImGuiWindowFlags_NoBackground |
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
	ImGui::SetNextWindowClass(&windowClass);

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	const Vector2f viewportPos = viewport->Pos;
	const Vector2f viewportSize = viewport->Size;
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