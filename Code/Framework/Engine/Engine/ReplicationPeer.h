#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Map.h"
#include "ECS/Entity.h"
#include "Network/Entity.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace net
{
	struct AddComponentMessage;
	struct CreateEntityMessage;
	struct DestroyEntityMessage;
	struct Message;
	struct PeerId;
	struct RemoveComponentMessage;
	struct UpdateComponentMessage;
}

namespace net
{
	class ReplicationPeer
	{
	public:
		ReplicationPeer(ecs::EntityWorld& entityWorld);

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	private:
		void OnClientDisconnected(const net::PeerId& peerId);
		void OnProcessMessages(const Array<const net::Message*>& messages);

		void OnCreateEntity(const net::CreateEntityMessage* message);
		void OnDestroyEntity(const net::DestroyEntityMessage* message);

		void OnAddComponent(const net::AddComponentMessage* message);
		void OnRemoveComponent(const net::RemoveComponentMessage* message);
		void OnUpdateComponent(const net::UpdateComponentMessage* message);

	private:
		ecs::EntityWorld& m_EntityWorld;

		Array<int32> m_Connections;
		DelegateCollection m_Collections;
		Map<ecs::Entity, net::Entity> m_PeerToHost;
		Map<net::Entity, ecs::Entity> m_HostToPeer;
		bool m_HasDisconnected = false;
	};
}