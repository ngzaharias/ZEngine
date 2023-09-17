#include <Catch2/catch.hpp>

#include <Core/Types.h>

#include <ECS/Component.h>
#include <ECS/EntityWorld.h>
#include <ECS/Manager.h>
#include <ECS/System.h>
#include <ECS/WorldView.h>

class GameTime
{
};

namespace
{
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

	class ManagerA : public ecs::Manager
	{
	public:
		void Initialise() override { m_IsInitialised = true; }
		void Shutdown() override { m_IsShutdown = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsShutdown = false;
	};

	class ManagerB : public ecs::Manager
	{
	public:
		void Initialise() override { m_IsInitialised = true; }
		void Shutdown() override { m_IsShutdown = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsShutdown = false;
	};

	class SystemA : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		void Initialise() override { m_IsInitialised = true; }
		void Shutdown() override { m_IsShutdown = true; }
		void Update(World& entityWorld, const GameTime& gameTime) { m_IsUpdated = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsShutdown = false;
		bool m_IsUpdated = false;
	};

	class SystemB : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		void Initialise() override { m_IsInitialised = true; }
		void Shutdown() override { m_IsShutdown = true; }
		void Update(World& entityWorld, const GameTime& gameTime) { m_IsUpdated = true; }

	public:
		bool m_IsInitialised = false;
		bool m_IsShutdown = false;
		bool m_IsUpdated = false;
	};
}

TEST_CASE("ecs::EntityWorld")
{
	SECTION("Register Components")
	{
		ecs::EntityWorld entityWorld;
		entityWorld.RegisterComponent<ComponentA>();
		entityWorld.RegisterComponent<ComponentB>();
	}

	SECTION("Register Managers")
	{
		ecs::EntityWorld entityWorld;
		entityWorld.RegisterManager<ManagerA>();
		entityWorld.RegisterManager<ManagerB>();
	}

	SECTION("Register Systems")
	{
		ecs::EntityWorld entityWorld;
		entityWorld.RegisterSystem<SystemA>();
		entityWorld.RegisterSystem<SystemB>();
	}

	SECTION("Initialise")
	{
		ecs::EntityWorld entityWorld;

		SECTION("Single Manager")
		{
			entityWorld.RegisterManager<ManagerA>();
			const auto& manager = entityWorld.GetManager<ManagerA>();

			CHECK(!manager.m_IsInitialised);

			entityWorld.Initialise();

			CHECK(manager.m_IsInitialised);
		}

		SECTION("Multiple Managers")
		{
			entityWorld.RegisterManager<ManagerA>();
			entityWorld.RegisterManager<ManagerB>();
			const auto& managerA = entityWorld.GetManager<ManagerA>();
			const auto& managerB = entityWorld.GetManager<ManagerB>();

			CHECK(!managerA.m_IsInitialised);
			CHECK(!managerB.m_IsInitialised);

			entityWorld.Initialise();

			CHECK(managerA.m_IsInitialised);
			CHECK(managerB.m_IsInitialised);
		}

		SECTION("Single System")
		{
			entityWorld.RegisterSystem<SystemA>();
			const auto& system = entityWorld.GetSystem<SystemA>();

			CHECK(!system.m_IsInitialised);

			entityWorld.Initialise();

			CHECK(system.m_IsInitialised);
		}

		SECTION("Multiple Systems")
		{
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
	}

	SECTION("Destroy")
	{
		ecs::EntityWorld entityWorld;

		SECTION("Single Manager")
		{
			entityWorld.RegisterManager<ManagerA>();
			const auto& manager = entityWorld.GetManager<ManagerA>();

			CHECK(!manager.m_IsShutdown);

			entityWorld.Shutdown();

			CHECK(manager.m_IsShutdown);
		}

		SECTION("Multiple Managers")
		{
			entityWorld.RegisterManager<ManagerA>();
			entityWorld.RegisterManager<ManagerB>();
			const auto& managerA = entityWorld.GetManager<ManagerA>();
			const auto& managerB = entityWorld.GetManager<ManagerB>();

			CHECK(!managerA.m_IsShutdown);
			CHECK(!managerB.m_IsShutdown);

			entityWorld.Shutdown();

			CHECK(managerA.m_IsShutdown);
			CHECK(managerB.m_IsShutdown);
		}

		SECTION("Single System")
		{
			entityWorld.RegisterSystem<SystemA>();
			entityWorld.Initialise();

			const auto& system = entityWorld.GetSystem<SystemA>();
			CHECK(!system.m_IsShutdown);

			entityWorld.Shutdown();

			CHECK(system.m_IsShutdown);
		}

		SECTION("Multiple Systems")
		{
			entityWorld.RegisterSystem<SystemA>();
			entityWorld.RegisterSystem<SystemB>();
			entityWorld.Initialise();

			const auto& systemA = entityWorld.GetSystem<SystemA>();
			const auto& systemB = entityWorld.GetSystem<SystemB>();
			CHECK(!systemA.m_IsShutdown);
			CHECK(!systemB.m_IsShutdown);

			entityWorld.Shutdown();

			CHECK(systemA.m_IsShutdown);
			CHECK(systemB.m_IsShutdown);
		}
	}

	SECTION("Update")
	{
		ecs::EntityWorld entityWorld;

		SECTION("Single System")
		{
			entityWorld.RegisterSystem<SystemA>();
			entityWorld.Initialise();

			const auto& system = entityWorld.GetSystem<SystemA>();
			CHECK(!system.m_IsUpdated);

			entityWorld.Update({});

			CHECK(system.m_IsUpdated);
		}

		SECTION("Multiple Systems")
		{
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
	}

	SECTION("CreateEntity")
	{
		ecs::EntityWorld entityWorld;
		CHECK(entityWorld.CreateEntity() == ecs::Entity(0));
		CHECK(entityWorld.CreateEntity() == ecs::Entity(1));
		CHECK(entityWorld.CreateEntity() == ecs::Entity(2));
		CHECK(entityWorld.CreateEntity() == ecs::Entity(3));
		CHECK(entityWorld.CreateEntity() == ecs::Entity(4));
	}

	SECTION("DestroyEntity")
	{
		ecs::EntityWorld entityWorld;
		ecs::Entity entity = entityWorld.CreateEntity();
		entityWorld.Update({});

		CHECK(entity.GetIndex() == 0);
		CHECK(entity.GetVersion() == 0);

		entityWorld.DestroyEntity(entity);
		entity = entityWorld.CreateEntity();
		entityWorld.Update({});

		CHECK(entity.GetIndex() == 1);
		CHECK(entity.GetVersion() == 0);

		entityWorld.DestroyEntity(entity);
		entity = entityWorld.CreateEntity();
		entityWorld.Update({});

		CHECK(entity.GetIndex() == 2);
		CHECK(entity.GetVersion() == 0);

		entity = entityWorld.CreateEntity();
		CHECK(entity.GetIndex() == 0);
		CHECK(entity.GetVersion() == 1);
	}

	SECTION("IsAlive")
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

	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();

	ecs::Entity entity = entityWorld.CreateEntity();

	SECTION("HasComponent")
	{
		CHECK(!entityWorld.HasComponent<ComponentA>(entity));
		CHECK(!entityWorld.HasComponent<ComponentB>(entity));

		entityWorld.AddComponent<ComponentA>(entity);
		entityWorld.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		CHECK(entityWorld.HasComponent<ComponentA>(entity));
		CHECK(entityWorld.HasComponent<ComponentB>(entity));
	}

	SECTION("GetComponent")
	{
		//CHECK_THROWS(entityWorld.GetComponent<ComponentA>(entity));
		//CHECK_THROWS(entityWorld.GetComponent<ComponentB>(entity));

		entityWorld.AddComponent<ComponentA>(entity);
		entityWorld.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		CHECK_NOTHROW(entityWorld.GetComponent<ComponentA>(entity));
		CHECK_NOTHROW(entityWorld.GetComponent<ComponentB>(entity));
	}

	SECTION("AddComponent")
	{
		SECTION("Single")
		{
			CHECK_NOTHROW(entityWorld.AddComponent<ComponentA>(entity));
			entityWorld.Update({});

			CHECK(entityWorld.HasComponent<ComponentA>(entity));
			CHECK(!entityWorld.HasComponent<ComponentB>(entity));
		}

		SECTION("Double")
		{
			entityWorld.AddComponent<ComponentA>(entity);
			entityWorld.Update({});
			CHECK(entityWorld.HasComponent<ComponentA>(entity));

			//entityWorld.AddComponent<ComponentA>(entity);
			//CHECK_THROWS(entityWorld.Update({}));
		}

		SECTION("Vardic Constructor")
		{
			const auto& componentA = entityWorld.AddComponent<ComponentA>(entity, true);
			const auto& componentB = entityWorld.AddComponent<ComponentB>(entity, -1337);
			CHECK(componentA.m_Bool == true);
			CHECK(componentB.m_Int32 == -1337);
		}
	}

	SECTION("RemoveComponent")
	{
		entityWorld.AddComponent<ComponentA>(entity);
		entityWorld.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		SECTION("Single")
		{
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

		SECTION("Double")
		{
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
	}

	SECTION("Add/Remove Component")
	{
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
}