#include "GameDebugPCH.h"
#include "GameDebug/NetworkSystem.h"

#include "Core/GameTime.h"
#include "Core/Parse.h"
#include "Math/Vector.h"
#include "ECS/DebugEvent.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameDebug/DebugNetworkWindowRequest.h"
#include "GameDebug/NetworkWindowComponent.h"
#include "Math/Math.h"
#include "Network/Host.h"
#include "Network/NetworkManager.h"
#include "Network/Peer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void debug::NetworkSystem::Initialise(World& world)
{
	world.AddEvent<debug::NetworkWindowRequest>();
}

void debug::NetworkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;
	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::NetworkWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Debug: Network Window");

		auto& window = world.AddComponent<debug::NetworkWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
	}

	for (auto&& view : world.Query<ecs::query::Include<debug::NetworkWindowComponent>>())
	{
		auto& window = view.WriteRequired<debug::NetworkWindowComponent>();
		const str::String label = std::format("Network Debugger : {}", window.m_Identifier);

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::Button("host: startup"))
			{
				auto& manager = world.WriteResource<net::NetworkManager>();
				manager.GetHost().Startup();
			}

			if (ImGui::Button("host: shutdown"))
			{
				auto& manager = world.WriteResource<net::NetworkManager>();
				manager.GetHost().Shutdown();
			}

			if (ImGui::Button("host: send message"))
			{
				auto& eventData = world.AddEvent<ecs::DebugEvent>();
				eventData.m_Data = "PING FROM HOST";
			}
			ImGui::SameLine();
			imgui::InputText("##message", window.m_Message);

			ImGui::Separator();

			if (ImGui::Button("peer: connect"))
			{
				auto& manager = world.WriteResource<net::NetworkManager>();
				manager.GetPeer().Connect();
			}

			if (ImGui::Button("peer: disconnect"))
			{
				auto& manager = world.WriteResource<net::NetworkManager>();
				manager.GetPeer().Disconnect();
			}

			if (ImGui::Button("peer: send message"))
			{
				auto& a = world.AddEvent<ecs::DebugEvent>();
				a.m_Data = "A";
				auto& b = world.AddEvent<ecs::DebugEvent>();
				b.m_Data = "B";
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}