#include "GameClientPCH.h"
#include "GameClient/ModalStateSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/ModalMessageComponent.h"

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

	using AddedQuery = ecs::query
		::Added<const gui::modal::MessageComponent>
		::Include<const gui::modal::MessageComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& component = view.ReadRequired<gui::modal::MessageComponent>();
		const char* title = GetSafeTitle(component.m_Title);
		ImGui::OpenPopup(title);
	}

	for (auto&& view : world.Query<ecs::query::Include<const gui::modal::MessageComponent>>())
	{
		const auto& component = view.ReadRequired<gui::modal::MessageComponent>();
		const char* title = GetSafeTitle(component.m_Title);
		if (ImGui::BeginPopupModal(title))
		{
			ImGui::Text(component.m_Message.c_str());

			if (ImGui::Button("Ok"))
			{
				world.DestroyEntity(view);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}