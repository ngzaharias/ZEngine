#include "ServerCursorPCH.h"
#include "ServerCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/WorldView.h"
#include "ServerCursor/CursorTransformSingleton.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void server::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Map<net::PeerId, Vector2f> requests;
	for (const auto& eventData : world.Events<shared::cursor::ClientTransformEvent>())
	{
		requests[eventData.m_PeerId] = eventData.m_Translate;
	}

	if (!requests.IsEmpty())
	{
		auto& singleton = world.WriteSingleton<server::cursor::TransformSingleton>();
		for (auto&& [peerId, translate] : requests)
		{
			ecs::Entity& entity = singleton.m_Peers[peerId];
			if (entity.IsUnassigned())
			{
				entity = world.CreateEntity();
				world.AddComponent<ecs::ReplicationComponent>(entity);
				auto& component = world.AddComponent<shared::cursor::TransformComponent>(entity);
				component.m_PeerId = peerId;
				component.m_Translate = translate;
			}
			else
			{
				auto& component = world.WriteComponent<shared::cursor::TransformComponent>(entity);
				component.m_Translate = translate;
			}
		}
	}
}
