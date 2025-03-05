#include "GameClientPCH.h"
#include "GameClient/ModalStateSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/ModalComponents.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	const char* GetSafeTitle(const str::String& title)
	{
		return !title.empty() ? title.c_str() : "<untitled>";
	}
}

void gui::modal::StateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const gui::modal::MessageComponent>>())
	{
		const auto& component = world.ReadComponent<gui::modal::MessageComponent>(entity);
		const char* title = GetSafeTitle(component.m_Title);
		ImGui::OpenPopup(title);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const gui::modal::MessageComponent>>())
	{
		const auto& component = world.ReadComponent<gui::modal::MessageComponent>(entity);
		const char* title = GetSafeTitle(component.m_Title);
		if (ImGui::BeginPopupModal(title))
		{
			ImGui::Text(component.m_Message.c_str());

			if (ImGui::Button("Ok"))
			{
				world.DestroyEntity(entity);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}