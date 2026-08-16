#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/Event.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EntityWorld. " name, "[ecs::EntityWorld]")

class GameTime { };

namespace
{
	int32 s_SystemLastUpdated = 0;
	bool s_IsSystemInitialised = false;
	bool s_IsSystemUpdated = false;
	bool s_IsSystemShutdown = false;

	struct Component final : public ecs::Component
	{
		Component() = default;
		Component(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct FComponent final : public ecs::FrameComponent 
	{ 
		FComponent() = default;
		FComponent(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct SComponent final : public ecs::SoloComponent 
	{ 
		SComponent() = default;
		SComponent(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct TComponent final : public ecs::StaticComponent 
	{ 
		TComponent() = default;
		TComponent(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct Event final : public ecs::Event 
	{ 
		bool m_Bool = false;
	};

	class Resource 
	{ 
	public: 
		bool m_Bool = false; 
	};

	class SystemA : public ecs::System
	{
	public:
		using World = ecs::WorldView::Write<Component>;
		void Update(World& world, const GameTime& gameTime) { s_SystemLastUpdated = 1; }
	};

	class SystemB : public ecs::System
	{
	public:
		using World = ecs::WorldView::Read<Component>;
		void Update(World& world, const GameTime& gameTime) { s_SystemLastUpdated = 2; }
	};

	class SystemC : public ecs::System
	{
	public:
		using World = ecs::WorldView;

		void Initialise(World& world) { s_IsSystemInitialised = true; }
		void Shutdown(World& world) { s_IsSystemShutdown = true; }
		void Update(World& world, const GameTime& gameTime) { s_IsSystemUpdated = true; }
	};
}

CLASS_TEST_CASE("IsRegistered will return true for a Component that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	CHECK(world.IsRegistered<Component>());
}

CLASS_TEST_CASE("IsRegistered will return false for a Component that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<Component>());
}

CLASS_TEST_CASE("IsRegistered will return true for a FrameComponent that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<FComponent>();
	CHECK(world.IsRegistered<FComponent>());
}

CLASS_TEST_CASE("IsRegistered will return false for a FrameComponent that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<FComponent>());
}

CLASS_TEST_CASE("IsRegistered will return true for a SoloComponent that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	CHECK(world.IsRegistered<SComponent>());
}

CLASS_TEST_CASE("IsRegistered will return false for a SoloComponent that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<SComponent>());
}

CLASS_TEST_CASE("IsRegistered will return true for a StaticComponent that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();
	CHECK(world.IsRegistered<TComponent>());
}

CLASS_TEST_CASE("IsRegistered will return false for a StaticComponent that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<TComponent>());
}

CLASS_TEST_CASE("IsRegistered will return true for an Event that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterEvent<Event>();
	CHECK(world.IsRegistered<Event>());
}

CLASS_TEST_CASE("IsRegistered will return false for an Event that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<Event>());
}

CLASS_TEST_CASE("IsRegistered will return true for a Resource that is registered.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterResource(resource);
	CHECK(world.IsRegistered<Resource>());
}

CLASS_TEST_CASE("IsRegistered will return false for a Resource that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<Resource>());
}

CLASS_TEST_CASE("IsRegistered will return true for a System that is registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();
	CHECK(world.IsRegistered<SystemC>());
}

CLASS_TEST_CASE("IsRegistered will return false for a System that isn't registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK(!world.IsRegistered<SystemC>());
}

CLASS_TEST_CASE("Initialise will call initialise on registered systems.")
{
	s_IsSystemInitialised = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();

	world.Initialise();
	CHECK(s_IsSystemInitialised);
}

CLASS_TEST_CASE("Initialise won't call initialise on unregistered systems.")
{
	s_IsSystemInitialised = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);

	world.Initialise();
	CHECK(!s_IsSystemInitialised);
}

CLASS_TEST_CASE("Shutdown will call shutdown on registered systems.")
{
	s_IsSystemShutdown = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();
	world.Initialise();

	world.Shutdown();
	CHECK(s_IsSystemShutdown);
}

CLASS_TEST_CASE("Shutdown won't call shutdown if the world wasn't initialised.")
{
	s_IsSystemShutdown = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();

	world.Shutdown();
	CHECK(!s_IsSystemShutdown);
}

CLASS_TEST_CASE("Shutdown won't call shutdown on unregistered systems.")
{
	s_IsSystemShutdown = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.Initialise();

	world.Shutdown();
	CHECK(!s_IsSystemShutdown);
}

CLASS_TEST_CASE("Update will call update on registered systems.")
{
	s_IsSystemUpdated = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();
	world.Initialise();

	world.Update({});
	CHECK(s_IsSystemUpdated);
}

CLASS_TEST_CASE("Update won't call update on unregistered systems.")
{
	s_IsSystemUpdated = false;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.Initialise();

	world.Update({});
	CHECK(!s_IsSystemUpdated);
}

CLASS_TEST_CASE("Update will call update on systems that write to a component before a system that reads from the component.")
{
	s_SystemLastUpdated = 0;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	world.RegisterSystem<SystemA>();
	world.RegisterSystem<SystemB>();
	world.Initialise();

	world.Update({});
	CHECK(s_SystemLastUpdated == 2);
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was just created.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity that was created and then the the world is updated.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity after it was just destroyed.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	world.DestroyEntity(entity);
	CHECK(world.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was destroyed and then the the world is updated.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();

	world.Update({});
	world.DestroyEntity(entity);
	world.Update({});
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("CreateEntity constructs a valid entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();
	CHECK(!entity.IsUnassigned());
}

CLASS_TEST_CASE("DestroyEntity marks an entity for destruction.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	CHECK_NOTHROW(world.DestroyEntity(entity));
}

CLASS_TEST_CASE("DestroyEntity will only destroy the entity after update was called.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	world.DestroyEntity(entity);
	world.Update({});
	CHECK(!world.IsAlive(entity));
}

CLASS_TEST_CASE("DestroyEntity doesn't change the values of an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	ecs::Entity entity = world.CreateEntity();
	world.Update({});

	world.DestroyEntity(entity);
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);
}

CLASS_TEST_CASE("RegisterComponent will register a Component with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterComponent<Component>());
}

CLASS_TEST_CASE("RegisterComponent will register a FrameComponent with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterComponent<FComponent>());
}

CLASS_TEST_CASE("RegisterComponent will register a SoloComponent with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterComponent<SComponent>());
}

CLASS_TEST_CASE("RegisterComponent will register a StaticComponent with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterComponent<TComponent>());
}

CLASS_TEST_CASE("RegisterComponent will crash if the same component is registered twice.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	//CHECK_THROWS(world.RegisterComponent<Component>());
}

CLASS_TEST_CASE("AddComponent can add a component to an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("AddComponent can add a solo component.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});
	CHECK(world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("AddComponent can construct a component using vardic arguments.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	const auto& component = world.AddComponent<Component>(entity, true);
	CHECK(component.m_Bool == true);
}

CLASS_TEST_CASE("AddComponent can construct a solo component using vardic arguments.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	const auto& component = world.AddComponent<SComponent>(true);
	CHECK(component.m_Bool == true);
}

CLASS_TEST_CASE("AddComponent will crash the game if called twice on the same entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	//CHECK_THROWS(world.AddComponent<Component>(entity));
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("AddComponent will crash the game if called twice on the same solo component.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	//CHECK_THROWS(world.AddComponent<SComponent>());
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("AddComponent can be called again after the component was removed.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
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

CLASS_TEST_CASE("AddComponent can be called again after the solo component was removed.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});

	world.RemoveComponent<SComponent>();
	world.Update({});

	world.AddComponent<SComponent>();
	world.Update({});

	CHECK(world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("RemoveComponent will remove the component from an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	world.AddComponent<Component>(entity);
	world.Update({});

	CHECK_NOTHROW(world.RemoveComponent<Component>(entity));
	CHECK_NOTHROW(world.Update({}));
}

CLASS_TEST_CASE("RemoveComponent will remove the solo component.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});

	CHECK_NOTHROW(world.RemoveComponent<SComponent>());
	CHECK_NOTHROW(world.Update({}));
}

CLASS_TEST_CASE("RemoveComponent will crash the game if called on an entity that doesn't have the component.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	//CHECK_THROWS(world.RemoveComponent<Component>(entity));
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("RemoveComponent will crash the game if called on the solo component that wasn't added.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	//CHECK_THROWS(world.RemoveComponent<SComponent>());
	//CHECK_THROWS(world.Update({}));
}

CLASS_TEST_CASE("HasComponent returns true if a component has been added to an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	world.AddComponent<Component>(entity);
	world.Update({});

	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns true if the solo component has been added.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});

	CHECK(world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("HasComponent returns false if a component hasn't been added to an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if the solo component hasn't been added to an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	CHECK(!world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("HasComponent returns true if a component was removed but the world hasn't updated yet.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	world.RemoveComponent<Component>(entity);
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns true if the solo component was removed but the world hasn't updated yet.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});
	world.RemoveComponent<SComponent>();
	CHECK(world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("HasComponent returns false if a component was added but the world hasn't updated yet.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	CHECK(!world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if the solo component was added but the world hasn't updated yet.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	CHECK(!world.HasComponent<SComponent>());
}

CLASS_TEST_CASE("ReadComponent returns a Component that can't be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	auto& component = world.ReadComponent<Component>(entity);
	// component.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadComponent returns a FrameComponent that can't be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<FComponent>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<FComponent>(entity);
	world.Update({});
	auto& component = world.ReadComponent<FComponent>(entity);
	// component.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadComponent returns the SoloComponent that can't be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});
	auto& component = world.ReadComponent<SComponent>();
	// component.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadComponent returns a StaticComponent that can't be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();

	auto& component = world.ReadComponent<TComponent>();
	// component.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadComponent crashes when the Component hasn't been added to the entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.ReadComponent<Component>(entity));
}

CLASS_TEST_CASE("ReadComponent crashes when the FrameComponent hasn't been added to the entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.ReadComponent<Component>(entity));
}

CLASS_TEST_CASE("ReadComponent crashes when the SoloComponent hasn't been added.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	//CHECK_THROWS(world.ReadComponent<SComponent>());
}

CLASS_TEST_CASE("ReadComponent crashes when the StaticComponent hasn't been registerd.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();
	world.Initialise();

	//CHECK_THROWS(world.ReadComponent<SComponent>());
}

CLASS_TEST_CASE("WriteComponent returns a Component that can be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});

	auto& component = world.WriteComponent<Component>(entity);
	component.m_Bool = true;
	CHECK(component.m_Bool);
}

CLASS_TEST_CASE("WriteComponent returns a FrameComponent that can be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<FComponent>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<FComponent>(entity);
	world.Update({});

	auto& component = world.WriteComponent<FComponent>(entity);
	component.m_Bool = true;
	CHECK(component.m_Bool);
}

CLASS_TEST_CASE("WriteComponent returns a SoloComponent that can be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<SComponent>();
	world.Initialise();

	world.AddComponent<SComponent>();
	world.Update({});

	auto& component = world.WriteComponent<SComponent>();
	component.m_Bool = true;
	CHECK(component.m_Bool);
}

CLASS_TEST_CASE("WriteComponent returns a StaticComponent that can be modified.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();
	world.Initialise();
	world.Update({});

	auto& component = world.WriteComponent<TComponent>();
	component.m_Bool = true;
	CHECK(component.m_Bool);
}

CLASS_TEST_CASE("WriteComponent crashes when the Component hasn't been added to the entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.WriteComponent<Component>(entity));
}

CLASS_TEST_CASE("WriteComponent crashes when the FrameComponent hasn't been added to the entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<FComponent>();
	ecs::Entity entity = world.CreateEntity();

	//CHECK_THROWS(world.WriteComponent<FComponent>(entity));
}

CLASS_TEST_CASE("WriteComponent crashes when the SoloComponent hasn't been added.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();
	world.Initialise();

	//CHECK_THROWS(world.WriteComponent<SComponent>());
}

CLASS_TEST_CASE("WriteComponent crashes when the StaticComponent hasn't been registered.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.Initialise();

	//CHECK_THROWS(world.WriteComponent<TComponent>());
}

CLASS_TEST_CASE("RegisterEvent will register an event with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterEvent<Event>());
}

CLASS_TEST_CASE("RegisterEvent will crash if the same event is registered twice.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterEvent<Event>();
	//CHECK_THROWS(world.RegisterEvent<Event>());
}

CLASS_TEST_CASE("AddEvent will add an event to the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterEvent<Event>();

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();

	int32 count = 0;
	world.AddEvent<Event>();
	world.Update({});
	for (const Event& eventData : view.Events<const Event>())
		count++;
	CHECK(count == 1);
}

CLASS_TEST_CASE("AddEvent will add an event to the world but the data won't be accessible before update is called.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterEvent<Event>();

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();

	int32 count = 0;
	world.AddEvent<Event>();
	for (const Event& eventData : view.Events<const Event>())
		count++;
	CHECK(count == 0);
}

CLASS_TEST_CASE("AddEvent can add multiple of the same event to the world at a time.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterEvent<Event>();

	world.AddEvent<Event>();
	world.AddEvent<Event>();
	world.Update({});

	using WorldView = ecs::WorldView::Read<Event>;
	WorldView view = world.WorldView<WorldView>();
	const auto& wrapper = view.Events<const Event>();
	CHECK(wrapper.m_Main.GetCount() == 2);
}

CLASS_TEST_CASE("AddEvent will crash the game if called on an unregistered event.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	//CHECK_THROWS(world.AddEvent<Event>());
}

CLASS_TEST_CASE("RegisterResource will register a resource with the world.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterResource(resource));
}

CLASS_TEST_CASE("RegisterResource will crash if the same resource is registered twice.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterResource(resource);
	//CHECK_THROWS(world.RegisterResource(resource));
}

CLASS_TEST_CASE("ReadResource returns a resource that can't be modified.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterResource(resource);

	auto& read = world.ReadResource<Resource>();
	// read.m_Bool = true; // doesn't compile
}

CLASS_TEST_CASE("ReadResource crashes when the resource hasn't been registered.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	//CHECK_THROWS(world.ReadResource<Resource>());
}

CLASS_TEST_CASE("WriteResource returns a resource that can be modified.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterResource(resource);

	auto& write = world.WriteResource<Resource>();
	write.m_Bool = true;
	CHECK(write.m_Bool);
}

CLASS_TEST_CASE("WriteResource crashes when the resource hasn't been registered.")
{
	Resource resource;
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	//CHECK_THROWS(world.WriteResource<Resource>());
}

CLASS_TEST_CASE("RegisterSystem will register a system with the world.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	CHECK_NOTHROW(world.RegisterSystem<SystemC>());
}

CLASS_TEST_CASE("RegisterSystem will crash if the same system is registered twice.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();
	//CHECK_THROWS(world.RegisterSystem<System>());
}

CLASS_TEST_CASE("GetSystem will return a registered system.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterSystem<SystemC>();
	CHECK_NOTHROW(world.GetSystem<SystemC>());
}

CLASS_TEST_CASE("GetSystem will crash the game if called on an unregistered system.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	//CHECK_THROWS(world.GetSystem<System>());
}

CLASS_TEST_CASE("FrameComponent is removed at the end of the next frame.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<FComponent>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<FComponent>(entity);
	world.Update({});
	world.Update({});
	CHECK(!world.HasComponent<FComponent>(entity));
}

CLASS_TEST_CASE("StaticComponent is available during initialise.")
{
	class System : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<TComponent>;

		void Initialise(World& world)
		{
			CHECK(world.HasComponent<TComponent>());
		};
	};

	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();
	world.RegisterSystem<System>();
	world.Initialise();
}

CLASS_TEST_CASE("StaticComponent is available during shutdown.")
{
	class System : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<TComponent>;

		void Shutdown(World& world)
		{
			CHECK(world.HasComponent<TComponent>());
		};
	};

	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<TComponent>();
	world.RegisterSystem<System>();
	world.Initialise();
	world.Shutdown();
}