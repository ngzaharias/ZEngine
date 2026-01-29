#include "ClientCursorPCH.h"
#include "ClientCursor/CursorTransformSystem.h"

#include "ClientCursor/CursorRenderComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/TransformComponent.h"
#include "Network/Peer.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void client::cursor::TransformSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<client::cursor::RenderComponent>(entity);
	world.AddComponent<eng::TransformComponent>(entity);
}

void client::cursor::TransformSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();
}

void client::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using SelfUpdateQuery = ecs::query
		::Include<
		eng::TransformComponent,
		const client::cursor::RenderComponent>
		::Exclude<
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<SelfUpdateQuery>())
	{
		const auto& input = world.ReadResource<eng::InputManager>();
		const auto& transform = view.ReadRequired<eng::TransformComponent>();
		const Vector2f& mousePos = input.m_MousePosition;
		const Vector3f& translate = transform.m_Translate;
		if (mousePos.x != translate.x || mousePos.y != translate.y)
		{
			auto& writeTransform = view.WriteRequired<eng::TransformComponent>();
			writeTransform.m_Translate.x = mousePos.x;
			writeTransform.m_Translate.y = mousePos.y;

			const auto& peer = world.ReadResource<net::Peer>();
			const net::PeerId peerId = peer.GetPeerId();
			if (peer.IsConnected() && peerId != net::PeerId::Unassigned)
			{
				auto& eventData = world.AddEvent<shared::cursor::ClientTransformEvent>();
				eventData.m_PeerId = peer.GetPeerId();
				eventData.m_Translate = mousePos;
			}
		}
	}

	using PeerAddedQuery = ecs::query
		::Added<
		const shared::cursor::TransformComponent>
		::Include<
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<PeerAddedQuery>())
	{
		const auto& peer = world.ReadResource<net::Peer>();
		const net::PeerId peerId = peer.GetPeerId();
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		if (serverComponent.m_PeerId == peerId)
			continue;

		Z_LOG(ELog::Debug, "Cursor: Added Cursor for PeerId {}", peerId.GetIndex());

		world.AddComponent<client::cursor::RenderComponent>(view);

		auto& clientComponent = world.AddComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y;
	}

	using PeerUpdateQuery = ecs::query
		::Updated<
		const shared::cursor::TransformComponent>
		::Include<
		const eng::TransformComponent,
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<PeerUpdateQuery>())
	{
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		auto& clientComponent = world.WriteComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y;
	}
}
