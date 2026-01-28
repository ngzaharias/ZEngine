#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/ReplicationPeer.h"
#include "ECS/TypeRegistry.h"
#include "ECS/WorldView.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::ReplicationPeer. " name, "[ecs::ReplicationPeer]")

namespace
{
	struct Component final : public ecs::Component<Component>
	{
		Component() = default;
		Component(int32 val) : m_Value(val) { }
		int32 m_Value = 1;
	};

	struct RAIIWrapper
	{
		RAIIWrapper() : m_Peer(m_World)
		{
			m_World.RegisterResource(m_Registry);
			m_World.RegisterComponent<Component>();
			m_Registry.RegisterComponent<Component>();
			m_World.Initialise();
		}

		ecs::EntityWorld m_World;
		ecs::TypeRegistry m_Registry;
		ecs::ReplicationPeer m_Peer;
	};
}

CLASS_TEST_CASE("OnEntityCreate will create a new entity.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	ecs::EntityCreateMessage message;
	message.m_Entity = nentity;
	message.m_Type = ecs::EMessage::EntityCreate;
	peer.OnEntityCreate(&message);

	world.Update({});
	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("OnEntityCreate and OnEntityDestroy called on the same entity in the same frame will result in no entity being created.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::EntityDestroyMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityDestroy;
		peer.OnEntityDestroy(&message);
	}

	world.Update({});
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("OnEntityCreate and CreateEntity called in the same frame will result in two different entities being created.")
{
	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	net::Entity nentity(0);
	ecs::Entity entityA = world.CreateEntity();
	ecs::Entity entityB(1);

	ecs::EntityCreateMessage message;
	message.m_Entity = nentity;
	message.m_Type = ecs::EMessage::EntityCreate;
	peer.OnEntityCreate(&message);
	world.Update({});

	CHECK(world.IsAlive(entityA));
	CHECK(world.IsAlive(entityB));
}

CLASS_TEST_CASE("OnEntityCreate called after OnEntityDestroy in the same frame will result in no change to the entity.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::EntityDestroyMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityDestroy;
		peer.OnEntityDestroy(&message);
	}
	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});

	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("OnEntityDestroy will destroy the matching entity.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::EntityDestroyMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityDestroy;
		peer.OnEntityDestroy(&message);
	}
	world.Update({});

	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("OnEntityCreate and OnComponentAdd called in the same frame will result in the entity being alive and the component being added.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnEntityCreate and OnComponentAdd and OnEntityDestroy all called in the same frame will result in the entity being destroyed.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	{
		ecs::EntityDestroyMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityDestroy;
		peer.OnEntityDestroy(&message);
	}
	world.Update({});

	REQUIRE(!world.IsAlive(entity));
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnComponentAdd will add the component to the matching entity.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnComponentAdd will add the component with its data.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component(1337));
		peer.OnComponentAdd(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	REQUIRE(world.HasComponent<Component>(entity));
	const auto& component = world.ReadComponent<Component>(entity);
	CHECK(component.m_Value == 1337);
}

CLASS_TEST_CASE("OnComponentAdd and OnComponentRemove called in the same frame will result in the component not being added.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	world.Update({});
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	{
		ecs::ComponentRemoveMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentRemove;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		peer.OnComponentRemove(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnComponentAdd called after OnComponentRemove in the same frame will result in the component being added.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});
	{
		ecs::ComponentRemoveMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentRemove;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		peer.OnComponentRemove(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnComponentRemove will remove the component from the matching entity.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});
	{
		ecs::ComponentRemoveMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentRemove;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		peer.OnComponentRemove(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("OnComponentUpdate will update the data of the component.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});
	{
		ecs::ComponentUpdateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentUpdate;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component(1337));
		peer.OnComponentUpdate(&message);
	}
	world.Update({});

	REQUIRE(world.IsAlive(entity));
	REQUIRE(world.HasComponent<Component>(entity));
	const auto& component = world.ReadComponent<Component>(entity);
	CHECK(component.m_Value == 1337);
}

CLASS_TEST_CASE("OnComponentUpdate will mark the component as updated in queries.")
{
	net::Entity nentity(0);
	ecs::Entity entity(0);

	RAIIWrapper raii;
	ecs::EntityWorld& world = raii.m_World;
	ecs::ReplicationPeer& peer = raii.m_Peer;

	{
		ecs::EntityCreateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::EntityCreate;
		peer.OnEntityCreate(&message);
	}
	{
		ecs::ComponentAddMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentAdd;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentAdd(&message);
	}
	world.Update({});
	{
		ecs::ComponentUpdateMessage message;
		message.m_Entity = nentity;
		message.m_Type = ecs::EMessage::ComponentUpdate;
		message.m_TypeId = ToTypeId<Component, ecs::ComponentTag>();
		message.m_Data.Write(Component{});
		peer.OnComponentUpdate(&message);
	}
	world.Update({});

	int32 count = 0;
	ecs::WorldView worldView = world.WorldView<ecs::WorldView>();
	for (auto&& view : worldView.Query<ecs::query::Updated<Component>>())
	{
		CHECK(view.GetEntity() == entity);
		count++;
	}
	CHECK(count == 1);
}