#include <Catch2/catch.hpp>

#include "ECS/Event.h"
#include "ECS/EventBuffer.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EventBuffer. " name, "[ecs::EventBuffer]")

namespace
{
	struct EventA final : public ecs::Event<EventA> { int32 m_Value = 0; };
	struct EventB final : public ecs::Event<EventB> { int32 m_Value = 0; };
}

CLASS_TEST_CASE("Registering an event constructs a container for the type.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	REQUIRE(buffer.TryAt<EventA>());
}

CLASS_TEST_CASE("Added events are stored inside an event container.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	auto& eventData = buffer.AddEvent<EventA>();
	eventData.m_Value = 1;

	const ecs::EventContainer<EventA>& container = buffer.GetAt<EventA>();
	REQUIRE(container.GetCount() == 1);
	CHECK(container.GetValues()[0].m_Value == 1);
}

CLASS_TEST_CASE("Events are stored in the order they were added in.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	auto& eventA = buffer.AddEvent<EventA>();
	eventA.m_Value = 1;
	auto& eventB = buffer.AddEvent<EventA>();
	eventB.m_Value = 2;

	const ecs::EventContainer<EventA>& container = buffer.GetAt<EventA>();
	REQUIRE(container.GetCount() == 2);
	CHECK(container.GetValues()[0].m_Value == 1);
	CHECK(container.GetValues()[1].m_Value == 2);
}

CLASS_TEST_CASE("GetAll returns all registered containers.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();
	buffer.RegisterEvent<EventB>();

	const SparseArray<ecs::EventId, ecs::IEventContainer*>& containers = buffer.GetAll();
	CHECK(containers.GetCount() == 2);
}

CLASS_TEST_CASE("GetAt returns the container interface that matches the typeId.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	const ecs::EventId typeId = ToTypeId<EventA, ecs::EventTag>();
	CHECK_NOTHROW(buffer.GetAt(typeId));
}

CLASS_TEST_CASE("GetAt returns the container that matches the type.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	CHECK_NOTHROW(buffer.GetAt<EventA>());
}

CLASS_TEST_CASE("GetAt can't return a container interface using a typeId if it doesn't exist.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	const ecs::EventId typeId = ToTypeId<EventA, ecs::EventTag>();
	//CHECK_THROWS(buffer.GetAt(typeId));
}

CLASS_TEST_CASE("GetAt can't return a container using a type if it doesn't exist.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	//CHECK_THROWS(buffer.GetAt<EventA>());
}

CLASS_TEST_CASE("TryAt returns the container interface that matches the typeId.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	const ecs::EventId typeId = ToTypeId<EventA, ecs::EventTag>();
	CHECK(buffer.TryAt(typeId));
}

CLASS_TEST_CASE("TryAt returns the container that matches the type.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	CHECK(buffer.TryAt<EventA>());
}

CLASS_TEST_CASE("TryAt returns nullptr if there are no containers that match the typeId.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	const ecs::EventId typeId = ToTypeId<EventB, ecs::EventTag>();
	CHECK(!buffer.TryAt(typeId));
}

CLASS_TEST_CASE("TryAt returns nullptr if there are no containers that match the type.")
{
	ecs::EventBuffer buffer;
	buffer.RegisterEvent<EventA>();

	CHECK(!buffer.TryAt<EventB>());
}