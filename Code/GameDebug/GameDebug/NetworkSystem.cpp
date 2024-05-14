#include "GameDebugPCH.h"
#include "GameDebug/NetworkSystem.h"

#include <Core/GameTime.h>
#include <Core/Math.h>
#include <Core/Parse.h>
#include <Core/Vector.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <GameClient/GameStateComponents.h>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/NetworkComponents.h"

void dbg::NetworkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::NetworkWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Debug: Network Window");
		world.AddComponent<dbg::NetworkWindowComponent>(windowEntity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::NetworkWindowComponent>>())
	{
		const str::String label = "Network##" + std::to_string(entity.GetIndex());

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen))
		{
			auto& networkComponent = world.GetComponent<dbg::NetworkWindowComponent>(entity);

			ImGui::InputText("##client_address", &networkComponent.m_ClientAddress);
			ImGui::SameLine();
			ImGui::InputInt("##client_port", &networkComponent.m_ClientPort);

			ImGui::InputText("##server_address", &networkComponent.m_ServerAddress);
			ImGui::SameLine();
			ImGui::InputInt("##server_port", &networkComponent.m_ServerPort);

			if (ImGui::Button("Standalone"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::Standalone;
				request.m_ClientAddress = networkComponent.m_ClientAddress;
				request.m_ClientPort = networkComponent.m_ClientPort;
				request.m_ServerAddress = networkComponent.m_ServerAddress;
				request.m_ServerPort = networkComponent.m_ServerPort;

				auto& requestComponent = world.AddEventComponent<gamestate::RequestComponent>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Remote Client"))
			{
				gamestate::NetworkJoin request;
				request.m_Mode = net::EMode::RemoteClient;
				request.m_ClientAddress = networkComponent.m_ClientAddress;
				request.m_ClientPort = networkComponent.m_ClientPort;
				request.m_ServerAddress = networkComponent.m_ServerAddress;
				request.m_ServerPort = networkComponent.m_ServerPort;

				auto& requestComponent = world.AddEventComponent<gamestate::RequestComponent>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Listen Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::ListenServer;
				request.m_ClientAddress = networkComponent.m_ClientAddress;
				request.m_ClientPort = networkComponent.m_ClientPort;
				request.m_ServerAddress = networkComponent.m_ServerAddress;
				request.m_ServerPort = networkComponent.m_ServerPort;

				auto& requestComponent = world.AddEventComponent<gamestate::RequestComponent>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Dedicated Server"))
			{
				gamestate::NetworkHost request;
				request.m_Mode = net::EMode::DedicatedServer;
				request.m_ClientAddress = networkComponent.m_ClientAddress;
				request.m_ClientPort = networkComponent.m_ClientPort;
				request.m_ServerAddress = networkComponent.m_ServerAddress;
				request.m_ServerPort = networkComponent.m_ServerPort;

				auto& requestComponent = world.AddEventComponent<gamestate::RequestComponent>();
				requestComponent.m_Queue.Append(request);
			}

			if (ImGui::Button("Shutdown"))
			{
				auto& requestComponent = world.AddEventComponent<gamestate::RequestComponent>();
				requestComponent.m_Queue.Append(gamestate::NetworkStop{});
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(entity);
	}
}