#include "GameUIPCH.h"
#include "GameUI/HiddenCountSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Hidden/HiddenCountComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void gui::hidden::CountSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<::hidden::CountComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoScrollWithMouse;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		Vector2f position = viewport->Pos;
		Vector2f size = viewport->Size;
		position.x += size.x * 0.5f;
		position.y += 100.f;

		ImGui::SetNextWindowPos(position);
		if (ImGui::Begin("Hidden Counter", nullptr, s_WindowFlags))
		{
			const auto& counter = world.ReadComponent<::hidden::CountComponent>(entity);
			ImGui::Text(" %d / %d ", counter.m_Revealed, counter.m_Objects);
		}
		ImGui::End();
	}
}
