#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

class GameTime { };

namespace
{
	bool m_IsSystemAShutdown = false;
	bool m_IsSystemBShutdown = false;

	struct ComponentA : public ecs::Component<ComponentA>
	{
		ComponentA() = default;
		ComponentA(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct ComponentB : public ecs::Component<ComponentB>
	{
		ComponentB() = default;
		ComponentB(int32 val) : m_Int32(val) { }
		int32 m_Int32 = 0;
	};

	class ResourceA { };
	class ResourceB { };

	class SystemA : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		void Initialise() override { m_IsInitialised = true; m_IsSystemAShutdown = false; }
		void Shutdown() override { m_IsSystemAShutdown = true; }
		void Update(World& entityWorld, const GameTime& gameTime) { m_IsUpdated = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsUpdated = false;
	};

	class SystemB : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		void Initialise() override { m_IsInitialised = true; m_IsSystemBShutdown = false; }
		void Shutdown() override { m_IsSystemBShutdown = true; }
		void Update(World& entityWorld, const GameTime& gameTime) { m_IsUpdated = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsUpdated = false;
	};
}

TEST_CASE("ecs::EntityWorld. Register Components.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
}

TEST_CASE("ecs::EntityWorld. Register Systems.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.RegisterSystem<SystemB>();
}

TEST_CASE("ecs::EntityWorld. Initialise a single system.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	const auto& system = entityWorld.GetSystem<SystemA>();

	CHECK(!system.m_IsInitialised);

	entityWorld.Initialise();

	CHECK(system.m_IsInitialised);
}

TEST_CASE("ecs::EntityWorld. Initialise multiple systems.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.RegisterSystem<SystemB>();
	const auto& systemA = entityWorld.GetSystem<SystemA>();
	const auto& systemB = entityWorld.GetSystem<SystemB>();

	CHECK(!systemA.m_IsInitialised);
	CHECK(!systemB.m_IsInitialised);

	entityWorld.Initialise();

	CHECK(systemA.m_IsInitialised);
	CHECK(systemB.m_IsInitialised);
}

TEST_CASE("ecs::EntityWorld. Destroy a single system.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.Initialise();

	const auto& system = entityWorld.GetSystem<SystemA>();
	CHECK(!m_IsSystemAShutdown);

	entityWorld.Shutdown();

	CHECK(m_IsSystemAShutdown);
}

TEST_CASE("ecs::EntityWorld. Destroy multiple systems.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.RegisterSystem<SystemB>();
	entityWorld.Initialise();

	const auto& systemA = entityWorld.GetSystem<SystemA>();
	const auto& systemB = entityWorld.GetSystem<SystemB>();
	CHECK(!m_IsSystemAShutdown);
	CHECK(!m_IsSystemBShutdown);

	entityWorld.Shutdown();

	CHECK(m_IsSystemAShutdown);
	CHECK(m_IsSystemBShutdown);
}

TEST_CASE("ecs::EntityWorld. Update single system.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.Initialise();

	const auto& system = entityWorld.GetSystem<SystemA>();
	CHECK(!system.m_IsUpdated);

	entityWorld.Update({});

	CHECK(system.m_IsUpdated);
}

TEST_CASE("ecs::EntityWorld. Update multiple systems.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterSystem<SystemA>();
	entityWorld.RegisterSystem<SystemB>();
	entityWorld.Initialise();

	const auto& systemA = entityWorld.GetSystem<SystemA>();
	const auto& systemB = entityWorld.GetSystem<SystemB>();
	CHECK(!systemA.m_IsUpdated);
	CHECK(!systemB.m_IsUpdated);

	entityWorld.Update({});

	CHECK(systemA.m_IsUpdated);
	CHECK(systemB.m_IsUpdated);
}

TEST_CASE("ecs::EntityWorld. CreateEntity.")
{
	ecs::EntityWorld entityWorld;
	CHECK(entityWorld.CreateEntity() == ecs::Entity(1));
	CHECK(entityWorld.CreateEntity() == ecs::Entity(2));
	CHECK(entityWorld.CreateEntity() == ecs::Entity(3));
	CHECK(entityWorld.CreateEntity() == ecs::Entity(4));
	CHECK(entityWorld.CreateEntity() == ecs::Entity(5));
}

TEST_CASE("ecs::EntityWorld. DestroyEntity.")
{
	ecs::EntityWorld entityWorld;
	ecs::Entity entity = entityWorld.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);

	entityWorld.DestroyEntity(entity);
	entity = entityWorld.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 2);
	CHECK(entity.GetVersion() == 0);

	entityWorld.DestroyEntity(entity);
	entity = entityWorld.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 3);
	CHECK(entity.GetVersion() == 0);

	entity = entityWorld.CreateEntity();
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 1);
}

TEST_CASE("ecs::EntityWorld. IsAlive.")
{
	ecs::Entity entity;
	ecs::EntityWorld entityWorld;

	CHECK(!entityWorld.IsAlive(entity));
	entity = entityWorld.CreateEntity();
	entityWorld.Update({});

	CHECK(entityWorld.IsAlive(entity));
	entityWorld.DestroyEntity(entity);
	entityWorld.Update({});

	CHECK(!entityWorld.IsAlive(entity));
}

TEST_CASE("ecs::EntityWorld. HasComponent.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	CHECK(!entityWorld.HasComponent<ComponentA>(entity));
	CHECK(!entityWorld.HasComponent<ComponentB>(entity));

	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK(entityWorld.HasComponent<ComponentA>(entity));
	CHECK(entityWorld.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::EntityWorld. GetComponent.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	//CHECK_THROWS(entityWorld.WriteComponent<ComponentA>(entity));
	//CHECK_THROWS(entityWorld.WriteComponent<ComponentB>(entity));

	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(entityWorld.WriteComponent<ComponentA>(entity));
	CHECK_NOTHROW(entityWorld.WriteComponent<ComponentB>(entity));
}

TEST_CASE("ecs::EntityWorld. Add a single component.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	CHECK_NOTHROW(entityWorld.AddComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(entityWorld.HasComponent<ComponentA>(entity));
	CHECK(!entityWorld.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::EntityWorld. Add multiple components.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.Update({});
	CHECK(entityWorld.HasComponent<ComponentA>(entity));

	//entityWorld.AddComponent<ComponentA>(entity);
	//CHECK_THROWS(entityWorld.Update({}));
}

TEST_CASE("ecs::EntityWorld. Add a component using vardic constructor.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	const auto& componentA = entityWorld.AddComponent<ComponentA>(entity, true);
	const auto& componentB = entityWorld.AddComponent<ComponentB>(entity, -1337);
	CHECK(componentA.m_Bool == true);
	CHECK(componentB.m_Int32 == -1337);
}

TEST_CASE("ecs::EntityWorld. Remove a single component.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(entityWorld.RemoveComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(!entityWorld.HasComponent<ComponentA>(entity, true));
	CHECK(entityWorld.HasComponent<ComponentA>(entity, false));
	CHECK(entityWorld.HasComponent<ComponentB>(entity, true));
	CHECK(!entityWorld.HasComponent<ComponentB>(entity, false));
	entityWorld.Update({});

	CHECK(!entityWorld.HasComponent<ComponentA>(entity, true));
	CHECK(!entityWorld.HasComponent<ComponentA>(entity, false));
	CHECK(entityWorld.HasComponent<ComponentB>(entity, true));
	CHECK(!entityWorld.HasComponent<ComponentB>(entity, false));
}

TEST_CASE("ecs::EntityWorld. Double.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(entityWorld.RemoveComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(!entityWorld.HasComponent<ComponentA>(entity, true));
	CHECK(entityWorld.HasComponent<ComponentA>(entity, false));
	entityWorld.Update({});

	CHECK(!entityWorld.HasComponent<ComponentA>(entity, true));
	CHECK(!entityWorld.HasComponent<ComponentA>(entity, false));

	//entityWorld.RemoveComponent<ComponentA>(entity);
	//CHECK_THROWS(entityWorld.Update({}));
}

TEST_CASE("ecs::EntityWorld. Add/Remove Component.")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	ecs::Entity entity = entityWorld.CreateEntity();

	CHECK(!entityWorld.HasComponent<ComponentA>(entity));
	entityWorld.AddComponent<ComponentA>(entity);

	CHECK(!entityWorld.HasComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(entityWorld.HasComponent<ComponentA>(entity));
	entityWorld.RemoveComponent<ComponentA>(entity);
	entityWorld.Update({});

	CHECK(!entityWorld.HasComponent<ComponentA>(entity));
	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.Update({});

	CHECK(entityWorld.HasComponent<ComponentA>(entity));
	entityWorld.Update({});
}
