#include "EditorPCH.h"
#include "Editor/OverlaySystem.h"

#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EditorIcons.h"
#include "Engine/CameraComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr Rotator s_AxisPosX = Rotator(0.f, +90.f, 0.f);
	constexpr Rotator s_AxisPosY = Rotator(-90.f, 0.f, 0.f);
	constexpr Rotator s_AxisPosZ = Rotator(0.f, 0.f, 0.f);

	bool ButtonIcon(const char* label, const char* tooltip, const editor::Icon& icon, const ImVec2& size, const bool active = false)
	{
		const bool result = imgui::ImageButton(label, active, icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::SetItemTooltip(tooltip);
		ImGui::PopStyleVar();
		return result;
	}
}

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
	ImGui::SetNextWindowPos(viewportPos + Vector2f(viewportSize.x - 128.f, 82.f));
	ImGui::SetNextWindowSize(Vector2f(100.f, 132.f));
	if (ImGui::Begin("Camera##editor", nullptr, s_Flags))
	{
		using CameraQuery = ecs::query
			::Include<
			const eng::ActiveComponent,
			const eng::CameraComponent,
			const eng::EditorComponent>;

		ecs::Entity cameraEntity = {};
		for (auto&& view : world.Query<CameraQuery>())
			cameraEntity = view;

		if (!cameraEntity.IsUnassigned())
		{
			Optional<eng::Projection> projection = {};
			const auto& camera = world.ReadComponent<eng::CameraComponent>(cameraEntity);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
			if (std::holds_alternative<eng::Orthographic>(camera.m_Projection))
			{
				if (ButtonIcon("##camera", "Orthographic Camera", icon::VIEW_ORTHO, ImVec2(22, 22)))
					projection = eng::Perspective{};
			}
			else
			{
				if (ButtonIcon("##camera", "Perspective Camera", icon::VIEW_PERSP, ImVec2(22, 22)))
					projection = eng::Orthographic{};
			}
			ImGui::SameLine();

			if (projection)
			{
				auto& write = world.WriteComponent<eng::CameraComponent>(cameraEntity);
				write.m_Projection = *projection;
			}

			Optional<Rotator> rotation = {};
			const bool isAlignedX = transform.m_Rotate == s_AxisPosX;
			if (ButtonIcon("##axis x", "Align Camera to X Axis", icon::AXIS_YZ, ImVec2(22, 22), isAlignedX))
				rotation = s_AxisPosX;

			ImGui::Dummy(ImVec2(30.f, 0.f));
			ImGui::SameLine();

			const bool isAlignedY = transform.m_Rotate == s_AxisPosY;
			if (ButtonIcon("##axis y", "Align Camera to Y Axis", icon::AXIS_XZ, ImVec2(22, 22), isAlignedY))
				rotation = Rotator(-90.f, 0.f, 0.f);

			ImGui::Dummy(ImVec2(30.f, 0.f));
			ImGui::SameLine();

			const bool isAlignedZ = transform.m_Rotate == s_AxisPosZ;
			if (ButtonIcon("##axis z", "Align Camera to Z Axis", icon::AXIS_XY, ImVec2(22, 22), isAlignedZ))
				rotation = Rotator(0.f, 0.f, 0.f);

			if (rotation)
			{
				auto& write = world.WriteComponent<eng::TransformComponent>(cameraEntity);
				write.m_Rotate = *rotation;
			}
		}
	}
	ImGui::End();
}
