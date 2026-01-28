#include <Catch2/catch.hpp>

#include "ECS/Event.h"
#include "ECS/EventContainer.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EventContainer. " name, "[ecs::EventContainer]")

namespace
{
	struct EventA final : public ecs::Event<EventA> 
	{ 
		int32 m_Value = 0; 
	};

	struct EventB final : public ecs::Event<EventB>, ecs::IsReplicated 
	{ 
		int32 m_Value = 0; 
	};
}

CLASS_TEST_CASE("Container starts empty when default constructed.")
{
	ecs::EventContainer<EventA> container;
	CHECK(container.GetCount() == 0);
}

CLASS_TEST_CASE("Events can be emplaced into the container.")
{
	ecs::EventContainer<EventA> container;

	container.Emplace();
	CHECK(container.GetCount() == 1);
}

CLASS_TEST_CASE("Events are stored in the order they were added in.")
{
	ecs::EventContainer<EventA> container;

	auto& eventA = container.Emplace();
	eventA.m_Value = 1;
	auto& eventB = container.Emplace();
	eventB.m_Value = 2;

	REQUIRE(container.GetCount() == 2);
	CHECK(container.GetValues()[0].m_Value == 1);
	CHECK(container.GetValues()[1].m_Value == 2);
}

CLASS_TEST_CASE("GetCount returns the number of elements that are stored in the container.")
{
	ecs::EventContainer<EventA> container;
	CHECK(container.GetCount() == 0);

	container.Emplace();
	CHECK(container.GetCount() == 1);

	container.Emplace();
	container.Emplace();
	CHECK(container.GetCount() == 3);
}

CLASS_TEST_CASE("GetValues returns the array of events that are stored.")
{
	ecs::EventContainer<EventA> container;
	container.Emplace();
	container.Emplace();
	container.Emplace();

	const Array<EventA>& events = container.GetValues();
	CHECK(events.GetCount() == 3);
}

CLASS_TEST_CASE("MoveAll transfers all events from one container into another.")
{
	ecs::EventContainer<EventA> containerA;
	ecs::EventContainer<EventA> containerB;
	
	containerA.Emplace();
	containerA.Emplace();
	containerA.Emplace();
	CHECK(containerA.GetCount() == 3);
	CHECK(containerB.GetCount() == 0);

	containerA.MoveAll(containerB);
	CHECK(containerA.GetCount() == 0);
	CHECK(containerB.GetCount() == 3);
}

CLASS_TEST_CASE("ReadAt writes the data of an event at an index to a MemBuffer.")
{
	ecs::EventContainer<EventB> container;
	{
		EventB& eventData = container.Emplace();
		eventData.m_Value = 1;
	}

	MemBuffer buffer;
	container.ReadAt(buffer, 0);
	{
		EventB eventData;
		buffer.Read(eventData);
		CHECK(eventData.m_Value == 1);
	}
}

CLASS_TEST_CASE("ReadAt doesn't write any data of an event if the event isn't replicated.")
{
	ecs::EventContainer<EventA> container;
	EventA& eventData = container.Emplace();
	eventData.m_Value = 1;

	MemBuffer buffer;
	container.ReadAt(buffer, 0);
	CHECK(buffer.GetCount() == 0);
}

CLASS_TEST_CASE("ReadAll writes every event stored to a MemBuffer.")
{
	ecs::EventContainer<EventB> container;
	{
		EventB& eventA = container.Emplace();
		eventA.m_Value = 1;
		EventB& eventB = container.Emplace();
		eventB.m_Value = 2;
	}

	MemBuffer buffer;
	container.ReadAll(buffer);
	REQUIRE(buffer.GetCount() == 20);
	{
		int32 count = 0;
		buffer.Read(count);
		CHECK(count == 2);

		EventB eventA, eventB;
		buffer.Read(eventA);
		buffer.Read(eventB);

		CHECK(eventA.m_Value == 1);
		CHECK(eventB.m_Value == 2);
	}
}

CLASS_TEST_CASE("RemoveAll removes all events from the buffer.")
{
	ecs::EventContainer<EventB> container;

	container.Emplace();
	container.Emplace();
	REQUIRE(container.GetCount() == 2);

	container.RemoveAll();
	REQUIRE(container.GetCount() == 0);
}