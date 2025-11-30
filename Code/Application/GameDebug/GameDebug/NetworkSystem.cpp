#include "GameDebugPCH.h"
#include "GameDebug/NetworkSystem.h"

#include "Core/GameTime.h"
#include "Core/Parse.h"
#include "Math/Vector.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/NetworkComponents.h"
#include "Math/Math.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"


void dbg::NetworkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;
	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<dbg::NetworkWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Debug: Network Window");

		auto& window = world.AddComponent<dbg::NetworkWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::NetworkWindowComponent>>())
	{
		auto& window = world.WriteComponent<dbg::NetworkWindowComponent>(entity);
		const str::String label = std::format("Network Debugger : {}", window.m_Identifier);

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen, s_WindowFlags))
		{
			ImGui::InputText("##client_address", &window.m_ClientAddress);
			ImGui::SameLine();
			ImGui::InputInt("##client_port", &window.m_ClientPort);

			ImGui::InputText("##server_address", &window.m_ServerAddress);
			ImGui::SameLine();
			ImGui::InputInt("##server_port", &window.m_ServerPort);

			if (ImGui::Button("Standalone"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::Standalone;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Remote Client"))
			{
				gamestate::NetworkJoin request;
				request.m_Mode = net::EMode::RemoteClient;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Listen Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::ListenServer;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Dedicated Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::DedicatedServer;
				request.m_ClientAddress = window.m_ClientAddress;
				request.m_ClientPort = window.m_ClientPort;
				request.m_ServerAddress = window.m_ServerAddress;
				request.m_ServerPort = window.m_ServerPort;

				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Shutdown"))
			{
				auto& requestComponent = world.AddEvent<gamestate::ChangeRequest>();
				requestComponent.m_Queue.Append(gamestate::NetworkStop{});
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(entity);
	}
}