#include "GameClientPCH.h"
#include "GameClient/ModalStateSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/ModalComponents.h"

#include <imgui/imgui.h>

void gui::modal::StateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const gui::modal::MessageComponent>>())
	{
		const auto& component = world.GetComponent<const gui::modal::MessageComponent>(entity);
		ImGui::OpenPopup(component.m_Title.c_str());
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const gui::modal::MessageComponent>>())
	{
		const auto& component = world.GetComponent<const gui::modal::MessageComponent>(entity);
		if (ImGui::BeginPopupModal(component.m_Title.c_str()))
		{
			ImGui::Text(component.m_Message.c_str());

			if (ImGui::Button("Ok"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}
}