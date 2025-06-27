#include "GameDebugPCH.h"
#include "GameDebug/OverlaySystem.h"

#include "Core/Colour.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ColourHelpers.h"
#include "Engine/VersionComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameDebug/FPSCounter.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imguizmo/ImGuizmo.h"

void dbg::OverlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_Flags =
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
	if (ImGui::Begin("Overlay", nullptr, s_Flags))
	{
		ImGui::NewLine();

		// version
		{
			const auto& component = world.ReadSingleton<eng::VersionComponent>();
			if (!component.m_Commit.empty())
			{
				ImGui::TextColored(Colour::Green, "%s : %s", component.m_Commit.c_str(), component.m_Branch.c_str());
			}
			else if (!component.m_Version.empty())
			{
				ImGui::TextColored(Colour::Green, "%s", component.m_Version.c_str());
			}
		}

		// resolution / refreshRate
		{
			const auto& windowManager = world.ReadResource<eng::WindowManager>();
			if (const eng::Window* window = windowManager.GetWindow(0))
			{
				const Vector2u& resolution = window->GetResolution();
				const int32 refreshRate = window->GetRefreshRate();
				ImGui::TextColored(Colour::Green, "%dx%d : %d", resolution.x, resolution.y, refreshRate);
			}
		}

		// fps
		{
			m_FPSCounter.AddFrame(gameTime.m_DeltaTime);

			const int32 iFPS = m_FPSCounter.GetAverage();
			const float fFPS = static_cast<float>(iFPS);
			const float mapped = math::Remap(fFPS, 15.f, 60.f, 0.f, 1.f);
			const float clamped = math::Clamp(mapped, 0.f, 1.f);
			const Colour colour = colour::LerpRGB(Colour::Red, Colour::Green, clamped);

			ImGui::TextColored(colour, "FPS: %d", iFPS);
		}
	}
	ImGui::End();
}
