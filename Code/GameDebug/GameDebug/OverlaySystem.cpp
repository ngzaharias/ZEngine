#include "GameDebugPCH.h"
#include "GameDebug/OverlaySystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ColourHelpers.h"
#include "Engine/Screen.h"
#include "Engine/VersionComponent.h"
#include "GameDebug/FPSCounter.h"
#include "Math/Math.h"
#include "Math/Vector.h"

#include <imgui/imgui.h>

namespace
{
	const Vector3f s_Red = Vector3f(255.f, 0.f, 0.f);
	const Vector3f s_Green = Vector3f(0.f, 255.f, 0.f);
}

void dbg::OverlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	return;

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

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ Screen::width, Screen::height });
	if (ImGui::Begin("Overlay", nullptr, s_Flags))
	{
		ImGui::NewLine();

		// version
		{
			const auto& component = world.ReadSingleton<eng::VersionComponent>();
			if (!component.m_Commit.empty())
			{
				ImGui::Text("%s : %s", component.m_Commit.c_str(), component.m_Branch.c_str());
			}
			else if (!component.m_Version.empty())
			{
				ImGui::Text("%s", component.m_Version.c_str());
			}
		}

		// resolution
		{
			ImGui::Text("%.0fx%.0f", Screen::width, Screen::height);
		}

		// fps
		{
			m_FPSCounter.AddFrame(gameTime.m_DeltaTime);

			const int32 iFPS = m_FPSCounter.GetAverage();
			const float fFPS = static_cast<float>(iFPS);
			const float mapped = math::Remap(fFPS, 15.f, 60.f, 0.f, 1.f);
			const float clamped = math::Clamp(mapped, 0.f, 1.f);
			const Vector3f colour = colour::LerpRGB(s_Red, s_Green, clamped);

			const ImVec4 iColuor = { colour.x, colour.y, colour.z, 1.f };
			ImGui::TextColored(iColuor, "FPS: %d", iFPS);
		}
	}
	ImGui::End();
}
