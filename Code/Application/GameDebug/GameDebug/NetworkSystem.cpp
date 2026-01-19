#include "GameDebugPCH.h"
#include "GameDebug/NetworkSystem.h"

#include "Core/GameTime.h"
#include "Core/Parse.h"
#include "Math/Vector.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/DebugNetworkWindowRequest.h"
#include "GameDebug/NetworkEvent.h"
#include "GameDebug/NetworkWindowComponent.h"
#include "Math/Math.h"
#include "Network/Host.h"
#include "Network/Peer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"


debug::NetworkSystem::NetworkSystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

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
				auto& host = world.WriteResource<net::Host>();
				host.Startup();
			}

			if (ImGui::Button("host: shutdown"))
			{
				auto& host = world.WriteResource<net::Host>();
				host.Shutdown();
			}

			if (ImGui::Button("host: send message"))
			{
				auto& request = m_ServerWorld.AddEvent<debug::NetworkEvent>();
				request.m_Data = "PING FROM HOST";
			}

			ImGui::Separator();

			if (ImGui::Button("peer: connect"))
			{
				auto& peer = world.WriteResource<net::Peer>();
				peer.Connect();
			}

			if (ImGui::Button("peer: disconnect"))
			{
				auto& peer = world.WriteResource<net::Peer>();
				peer.Disconnect();
			}

			if (ImGui::Button("peer: send message"))
			{
				auto& request = m_ClientWorld.AddEvent<debug::NetworkEvent>();
				request.m_Data = "PING FROM PEER";
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}

	{
		World clientWorld = m_ClientWorld.WorldView<World>();
		for (const auto& request : clientWorld.Events<debug::NetworkEvent>())
			Z_LOG(ELog::Debug, "Client: {}", request.m_Data);

		World serverWorld = m_ServerWorld.WorldView<World>();
		for (const auto& request : serverWorld.Events<debug::NetworkEvent>())
			Z_LOG(ELog::Debug, "Server: {}", request.m_Data);
	}
}