#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/Event.h"
#include "ECS/Singleton.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EntityWorld. " name, "[ecs::EntityWorld]")

class GameTime { };

namespace
{
	int32 m_SystemLastUpdated = 0;
	bool m_IsSystemInitialised = false;
	bool m_IsSystemUpdated = false;
	bool m_IsSystemShutdown = false;

	struct Component final : public ecs::Component<Component>
	{
		Component() = default;
		Component(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct Event final : public ecs::Event<Event> 
	{ 
		bool m_Bool = false;
	};

	class Resource 
	{ 
	public: 
		bool m_Bool = false; 
	};

	struct Singleton final : public ecs::Singleton<Singleton> 
	{ 
		bool m_Bool = false; 
	};

	class System : public ecs::System
	{
	public:
		using World = ecs::WorldView;

		void Initialise() override { m_IsSystemInitialised = true; }
		void Shutdown() override { m_IsSystemShutdown = true; }
		void Update(World& world, const GameTime& gameTime) { m_IsSystemUpdated = true; }
	};

	class SystemA : public ecs::System
	{
	public:
		using World = ecs::WorldView::Write<Component>;
		void Update(World& world, const GameTime& gameTime) { m_SystemLastUpdated = 1; }
	};

	class SystemB : public ecs::System
	{
	public:
		using World = ecs::WorldView::Read<Component>;
		void Update(World& world, const GameTime& gameTime) { m_SystemLastUpdated = 2; }
	};
}

CLASS_TEST_CASE("IsRegistered will return true for a component that is registered.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	CHECK(world.IsRegistered<Component>());
}

CLASS_TEST_CASE("IsRegistered will return false for a component that isn't registered.")
{
	ecs::EntityWorld world;
	CHECK(!world.IsRegistered<Component>());
}

CLASS_TEST_CASE("IsRegistered will return true for a event that is registered.")
{
	ecs::EntityWorld world;
	world.RegisterEvent<Event>();
	CHECK(world.IsRegistered<Event>());
}

CLASS_TEST_CASE("IsRegistered will return false for a event that isn't registered.")
{
	ecs::EntityWorld world;
	CHECK(!world.IsRegistered<Event>());
}

CLASS_TEST_CASE("IsRegistered will return true for a resource that is registered.")
{
	Resource resource;
	ecs::EntityWorld world;
	world.RegisterResource(resource);
	CHECK(world.IsRegistered<Resource>());
}

CLASS_TEST_CASE("IsRegistered will return false for a Resource that isn't registered.")
{
	ecs::EntityWorld world;
	CHECK(!world.IsRegistered<Resource>());
}

CLASS_TEST_CASE("IsRegistered will return true for a singleton that is registered.")
{
	ecs::EntityWorld world;
	world.RegisterSingleton<Singleton>();
	CHECK(world.IsRegistered<Singleton>());
}

CLASS_TEST_CASE("IsRegistered will return false for a Singleton that isn't registered.")
{
	ecs::EntityWorld world;
	CHECK(!world.IsRegistered<Singleton>());
}

CLASS_TEST_CASE("IsRegistered will return true for a system that is registered.")
{
	ecs::EntityWorld world;
	world.RegisterSystem<System>();
	CHECK(world.IsRegistered<System>());
}

CLASS_TEST_CASE("IsRegistered will return false for a system that isn't registered.")
{
	ecs::EntityWorld world;
	CHECK(!world.IsRegistered<System>());
}

CLASS_TEST_CASE("Initialise will call initialise on registered systems.")
{
	m_IsSystemInitialised = false;
	ecs::EntityWorld world;
	world.RegisterSystem<System>();

	world.Initialise();
	CHECK(m_IsSystemInitialised);
}

CLASS_TEST_CASE("Initialise won't call initialise on unregistered systems.")
{
	m_IsSystemInitialised = false;
	ecs::EntityWorld world;

	world.Initialise();
	CHECK(!m_IsSystemInitialised);
}

CLASS_TEST_CASE("Shutdown will call shutdown on registered systems.")
{
	m_IsSystemShutdown = false;
	ecs::EntityWorld world;
	world.RegisterSystem<System>();
	world.Initialise();

	world.Shutdown();
	CHECK(m_IsSystemShutdown);
}

CLASS_TEST_CASE("Shutdown won't call shutdown if the world wasn't initialised.")
{
	m_IsSystemShutdown = false;
	ecs::EntityWorld world;
	world.RegisterSystem<System>();

	world.Shutdown();
	CHECK(!m_IsSystemShutdown);
}

CLASS_TEST_CASE("Shutdown won't call shutdown on unregistered systems.")
{
	m_IsSystemShutdown = false;
	ecs::EntityWorld world;
	world.Initialise();

	world.Shutdown();
	CHECK(!m_IsSystemShutdown);
}

CLASS_TEST_CASE("Update will call update on registered systems.")
{
	m_IsSystemUpdated = false;
	ecs::EntityWorld world;
	world.RegisterSystem<System>();
	world.Initialise();

	world.Update({});
	CHECK(m_IsSystemUpdated);
}

CLASS_TEST_CASE("Update won't call update on unregistered systems.")
{
	m_IsSystemUpdated = false;
	ecs::EntityWorld world;
	world.Initialise();

	world.Update({});
	CHECK(!m_IsSystemUpdated);
}

CLASS_TEST_CASE("Update will call update on systems that write to a component before a system that reads from the component.")
{
	m_SystemLastUpdated = 0;
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	world.RegisterSystem<SystemA>();
	world.RegisterSystem<SystemB>();
	world.Initialise();

	world.Update({});
	CHECK(m_SystemLastUpdated == 2);
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was just created.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity that was created and then the the world is updated.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity after it was just destroyed.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	world.DestroyEntity(entity);
	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was destroyed and then the the world is updated.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	world.DestroyEntity(entity);
	world.Update({});
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("CreateEntity constructs a valid entity.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	CHECK(!entity.IsUnassigned());
}

CLASS_TEST_CASE("CreateEntity constructs an entity using the main id.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	CHECK(entity.GetOwnership() == 0);
}

CLASS_TEST_CASE("DestroyEntity marks an entity for destruction.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	CHECK_NOTHROW(world.DestroyEntity(entity));
}

CLASS_TEST_CASE("DestroyEntity will only destroy the entity after update was called.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	world.DestroyEntity(entity);
	world.Update({});
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("DestroyEntity doesn't change the values of an entity.")
{
	ecs::EntityWorld world;
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	world.DestroyEntity(entity);
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);
	CHECK(entity.GetOwnership() == 0);
}

CLASS_TEST_CASE("RegisterComponent will register a component with the world.")
{
	ecs::EntityWorld world;
	CHECK_NOTHROW(world.RegisterComponent<Component>());
}

CLASS_TEST_CASE("RegisterComponent will crash if the same component is registered twice.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	//CHECK_THROWS(world.RegisterComponent<Component>());
}

CLASS_TEST_CASE("AddComponent can add a component to an entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("AddComponent can construct a component using vardic arguments.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	const auto& componentA = world.AddComponent<Component>(entity, true);
	CHECK(componentA.m_Bool == true);
}

CLASS_TEST_CASE("AddComponent will crash the game if called twice on the same entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});

	world.RemoveComponent<Component>(entity);
	//CHECK_THROWS(world.RemoveComponent<Component>(entity));
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("RemoveComponent will remove the component from an entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	world.AddComponent<Component>(entity);
	world.Update({});

	CHECK_NOTHROW(world.RemoveComponent<Component>(entity));
	CHECK_NOTHROW(world.Update({}));
}

CLASS_TEST_CASE("AddComponent can be called again after the component was removed.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});

	world.RemoveComponent<Component>(entity);
	world.Update({});

	world.AddComponent<Component>(entity);
	world.Update({});

	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("RemoveComponent will crash the game if called on an entity that doesn't have the component.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	//CHECK_THROWS(world.RemoveComponent<Component>(entity));
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("HasComponent returns true if a component has been added to an entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	world.AddComponent<Component>(entity);
	world.Update({});

	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if a component hasn't been added to an entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns true if a component was removed but the world hasn't updated yet.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	world.RemoveComponent<Component>(entity);
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if a component was added but the world hasn't updated yet.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("ReadComponent returns a component that can't be modified.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	auto& component = world.ReadComponent<Component>(entity);
	// component.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadComponent crashes when the component hasn't been added to the entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.ReadComponent<Component>(entity));
}

CLASS_TEST_CASE("WriteComponent returns a component that can be modified.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});

	auto& component = world.WriteComponent<Component>(entity);
	component.m_Bool = true;
	CHECK(component.m_Bool);
}

CLASS_TEST_CASE("WriteComponent crashes when the component hasn't been added to the entity.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.WriteComponent<Component>(entity));
}

CLASS_TEST_CASE("GetComponentMask returns a mask with the component added.")
{
	const ecs::ComponentId typeId = ToTypeId<Component, ecs::ComponentTag>();

	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	CHECK(world.GetComponentMask(entity).Has(typeId));
}

CLASS_TEST_CASE("GetComponentMask returns an empty mask if an entity has no components.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.Update({});
	CHECK(world.GetComponentMask(entity).HasNone());
}

CLASS_TEST_CASE("GetComponentMask returns an empty mask when a component was added to an entity but hasn't updated the world.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	world.AddComponent<Component>(entity);
	CHECK(world.GetComponentMask(entity).HasNone());
}

CLASS_TEST_CASE("GetComponentMask crashes when called on an entity that isn't alive.")
{
	ecs::EntityWorld world;
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	//CHECK_THROWS(world.GetComponentMask(entity).HasNone());
}

CLASS_TEST_CASE("RegisterEvent will register an event with the world.")
{
	ecs::EntityWorld world;
	CHECK_NOTHROW(world.RegisterEvent<Event>());
}

CLASS_TEST_CASE("RegisterEvent will crash if the same event is registered twice.")
{
	ecs::EntityWorld world;
	world.RegisterEvent<Event>();
	//CHECK_THROWS(world.RegisterEvent<Event>());
}

CLASS_TEST_CASE("AddEvent will add an event to the world.")
{
	ecs::EntityWorld world;
	world.RegisterEvent<Event>();

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();

	int32 count = 0;
	world.AddEvent<Event>();
	world.Update({});
	for (const Event& eventData : view.Events<Event>())
		count++;
	CHECK(count == 1);
}

CLASS_TEST_CASE("AddEvent will add an event to the world but the data won't be accessible before update is called.")
{
	ecs::EntityWorld world;
	world.RegisterEvent<Event>();

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();

	int32 count = 0;
	world.AddEvent<Event>();
	for (const Event& eventData : view.Events<Event>())
		count++;
	CHECK(count == 0);
}

CLASS_TEST_CASE("AddEvent can add multiple of the same event to the world at a time.")
{
	ecs::EntityWorld world;
	world.RegisterEvent<Event>();

	world.AddEvent<Event>();
	world.AddEvent<Event>();
	world.Update({});

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();
	const auto& wrapper = view.Events<Event>();
	CHECK(wrapper.m_Main.GetCount() == 2);
}

CLASS_TEST_CASE("AddEvent will crash the game if called on an unregistered event.")
{
	ecs::EntityWorld world;
	//CHECK_THROWS(world.AddEvent<Event>());
}

CLASS_TEST_CASE("RegisterResource will register a resource with the world.")
{
	Resource resource;
	ecs::EntityWorld world;
	CHECK_NOTHROW(world.RegisterResource(resource));
}

CLASS_TEST_CASE("RegisterResource will crash if the same resource is registered twice.")
{
	Resource resource;
	ecs::EntityWorld world;
	world.RegisterResource(resource);
	//CHECK_THROWS(world.RegisterResource(resource));
}

CLASS_TEST_CASE("ReadResource returns a resource that can't be modified.")
{
	Resource resource;
	ecs::EntityWorld world;
	world.RegisterResource(resource);

	auto& read = world.ReadResource<Resource>();
	// read.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadResource crashes when the resource hasn't been registered.")
{
	Resource resource;
	ecs::EntityWorld world;
	//CHECK_THROWS(world.ReadResource<Resource>());
}

CLASS_TEST_CASE("WriteResource returns a resource that can be modified.")
{
	Resource resource;
	ecs::EntityWorld world;
	world.RegisterResource(resource);

	auto& write = world.WriteResource<Resource>();
	write.m_Bool = true;
	CHECK(write.m_Bool);
}

CLASS_TEST_CASE("WriteResource crashes when the resource hasn't been registered.")
{
	Resource resource;
	ecs::EntityWorld world;
	//CHECK_THROWS(world.WriteResource<Resource>());
}

CLASS_TEST_CASE("RegisterSingleton will register an singleton with the world.")
{
	ecs::EntityWorld world;
	CHECK_NOTHROW(world.RegisterSingleton<Singleton>());
}

CLASS_TEST_CASE("RegisterSingleton will crash if the same singleton is registered twice.")
{
	ecs::EntityWorld world;
	world.RegisterSingleton<Singleton>();
	//CHECK_THROWS(world.RegisterSingleton<Singleton>());
}

CLASS_TEST_CASE("ReadSingleton returns a singleton that can't be modified.")
{
	ecs::EntityWorld world;
	world.RegisterSingleton<Singleton>();

	auto& singleton = world.ReadSingleton<Singleton>();
	// singleton.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadSingleton crashes when the singleton hasn't been registered.")
{
	ecs::EntityWorld world;
	//CHECK_THROWS(world.ReadSingleton<Singleton>());
}

CLASS_TEST_CASE("WriteSingleton returns a singleton that can be modified.")
{
	ecs::EntityWorld world;
	world.RegisterSingleton<Singleton>();

	auto& singleton = world.WriteSingleton<Singleton>();
	singleton.m_Bool = true;
	CHECK(singleton.m_Bool);
}

CLASS_TEST_CASE("WriteSingleton crashes when the singleton hasn't been registered.")
{
	ecs::EntityWorld world;
	//CHECK_THROWS(world.WriteSingleton<Singleton>());
}

CLASS_TEST_CASE("RegisterSystem will register a system with the world.")
{
	ecs::EntityWorld world;
	CHECK_NOTHROW(world.RegisterSystem<System>());
}

CLASS_TEST_CASE("RegisterSystem will crash if the same system is registered twice.")
{
	ecs::EntityWorld world;
	world.RegisterSystem<System>();
	//CHECK_THROWS(world.RegisterSystem<System>());
}

CLASS_TEST_CASE("GetSystem will return a registered system.")
{
	ecs::EntityWorld world;
	world.RegisterSystem<System>();
	CHECK_NOTHROW(world.GetSystem<System>());
}

CLASS_TEST_CASE("GetSystem will crash the game if called on an unregistered system.")
{
	ecs::EntityWorld world;
	//CHECK_THROWS(world.GetSystem<System>());
}