#include "GameDebugPCH.h"
#include "GameDebug/ContainerSystem.h"

#include "Container/ContainerMemberAddRequestComponent.h"
#include "Container/ContainerMemberAddResultComponent.h"
#include "Container/ContainerMemberMoveRequestComponent.h"
#include "Container/ContainerMemberMoveResultComponent.h"
#include "Container/ContainerStorageComponent.h"
#include "Container/ContainerStorageCreateRequestComponent.h"
#include "Container/ContainerStorageCreateResultComponent.h"
#include "Container/ContainerStorageDestroyRequestComponent.h"
#include "Container/ContainerStorageDestroyResultComponent.h"
#include "Core/String.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/ContainerWindowComponent.h"
#include "GameDebug/DebugContainerWindowRequest.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	str::String ToString(const ecs::Entity& entity)
	{
		if (entity.IsUnassigned())
			return "Unassigned";

		return std::format("Index: {}, Version: {}", entity.GetIndex(), entity.GetVersion());
	}

	str::String ToMemberString(const ecs::Entity& entity)
	{
		if (entity.IsUnassigned())
			return "";

		return std::format("{}, {}", entity.GetIndex(), entity.GetVersion());
	}
}

void debug::ContainerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 350.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::ContainerWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();

		auto& window = world.AddComponent<debug::ContainerWindowComponent>(windowEntity);
		window.m_Label = ToLabel("Container Debugger", identifier);
		window.m_Identifier = identifier;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const debug::ContainerWindowComponent>>())
	{
		const auto& window = world.ReadComponent<debug::ContainerWindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	using UpdateQuery = ecs::query
		::Include<
		debug::ContainerWindowComponent>
		::Optional<
		const container::MemberAddResultComponent,
		const container::MemberMoveResultComponent,
		const container::StorageCreateResultComponent,
		const container::StorageDestroyResultComponent>;
	for (auto&& windowView : world.Query<UpdateQuery>())
	{
		auto& window = windowView.WriteRequired<debug::ContainerWindowComponent>();
		const ecs::Entity& storageSelected = window.m_Storage;

		if (const auto* resultComponent = windowView.ReadOptional<container::StorageCreateResultComponent>())
		{
			window.m_Storage = resultComponent->m_Storage;
			world.RemoveComponent<container::StorageCreateRequestComponent>(windowView);
		}

		if (windowView.HasOptional<container::StorageDestroyResultComponent>())
		{
			window.m_Storage = ecs::Entity::Unassigned;
			world.RemoveComponent<container::StorageDestroyRequestComponent>(windowView);
		}

		if (windowView.HasOptional<container::MemberAddResultComponent>())
			world.RemoveComponent<container::MemberAddRequestComponent>(windowView);
		if (windowView.HasOptional<container::MemberMoveResultComponent>())
			world.RemoveComponent<container::MemberMoveRequestComponent>(windowView);

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			if (ImGui::Button("Create Storage"))
				world.AddComponent<container::StorageCreateRequestComponent>(windowView);
			ImGui::SameLine();
			if (ImGui::Button("Destroy Storage"))
			{
				auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(windowView);
				requestComponent.m_Storage = window.m_Storage;
			}

			if (ImGui::Button("Create Member"))
			{
				const ecs::Entity memberEntity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(windowView);
				requestComponent.m_Storage = window.m_Storage;
				requestComponent.m_Member = memberEntity;
			}
			ImGui::SameLine();
			if (ImGui::Button("Destroy Member"))
			{
			}

			ImGui::Separator();

			const str::String previewLabel = ToString(window.m_Storage);
			if (ImGui::BeginCombo("##storage", previewLabel.c_str()))
			{
				if (ImGui::Selectable("Unassigned"))
					window.m_Storage = ecs::Entity::Unassigned;
				for (auto&& storageView : world.Query<ecs::query::Include<container::StorageComponent>>())
				{
					const str::String storageLabel = ToString(storageView);
					if (ImGui::Selectable(storageLabel.c_str()))
						window.m_Storage = storageView;
				}
				ImGui::EndCombo();
			}

			if (world.HasComponent<container::StorageComponent>(storageSelected))
			{
				const auto& storageComponent = world.ReadComponent<container::StorageComponent>(storageSelected);

				constexpr int32 s_Columns = 5;
				constexpr int32 s_Rows = 5;
				constexpr ImVec2 s_ItemSize = ImVec2(50.f, 50.f);
				constexpr ImVec2 s_TableSize = ImVec2(s_Columns * s_ItemSize.x + 10.f, s_Rows * s_ItemSize.y + 10.f);
				struct Payload { ecs::Entity m_Storage; ecs::Entity m_Member; };

				for (int32 i = 0; i < s_Columns * s_Rows; ++i)
				{
					ImGui::PushID(i);
					if (i % s_Rows != 0)
						ImGui::SameLine();

					ecs::Entity memberEntity;
					if (i < storageComponent.m_Members.GetCount())
					{
						auto iterator = storageComponent.m_Members.begin();
						std::advance(iterator, i);
						memberEntity = *iterator;
					}

					const str::String memberLabel = ToMemberString(memberEntity);
					ImGui::Button(memberLabel.c_str(), s_ItemSize);

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						Payload payload = { storageSelected, memberEntity };
						ImGui::SetDragDropPayload("storage_payload", &payload, sizeof(Payload));
						ImGui::Text("[ ]");
						ImGui::EndDragDropSource();
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("storage_payload"))
						{
							const Payload data = *(const Payload*)payload->Data;

							if (!data.m_Member.IsUnassigned())
							{
								auto& requestComponent = world.AddComponent<container::MemberMoveRequestComponent>(windowView);
								requestComponent.m_Storage = storageSelected;
								requestComponent.m_Member = data.m_Member;
							}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();
				}
			}
			ImGui::End();

			if (!isWindowOpen)
				world.DestroyEntity(windowView);
		}
	}
}