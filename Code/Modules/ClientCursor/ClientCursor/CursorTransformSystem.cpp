#include "ClientCursorPCH.h"
#include "ClientCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Network/Peer.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void client::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<
		const shared::cursor::TransformComponent>
		::Include<
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& peer = world.ReadResource<net::Peer>();
		const net::PeerId peerId = peer.GetPeerId();
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		if (serverComponent.m_PeerId == peerId)
			continue;

		Z_LOG(ELog::Debug, "Cursor: Added Cursor for PeerId {}", peerId.GetIndex());

		world.AddComponent<eng::SpriteComponent>(view);

		auto& clientComponent = world.AddComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y;
	}

	using UpdateQuery = ecs::query
		::Updated<
		const shared::cursor::TransformComponent>
		::Include<
		const eng::TransformComponent,
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<UpdateQuery>())
	{
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		auto& clientComponent = world.WriteComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y;
	}
}
