#include <Catch2/catch.hpp>

#include "ECS/Event.h"
#include "ECS/EventStorage.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EventStorage. " name, "[ecs::EventStorage]")

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

CLASS_TEST_CASE("Registering a regular event registers it with just the main and next buffers.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();

	CHECK(storage.GetMainBuffer().TryAt<EventA>());
	CHECK(storage.GetNextBuffer().TryAt<EventA>());
	CHECK(!storage.GetSyncBuffer().TryAt<EventA>());
}

CLASS_TEST_CASE("Registering a replicated event registers it with all of the buffers.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventB>();

	CHECK(storage.GetMainBuffer().TryAt<EventB>());
	CHECK(storage.GetNextBuffer().TryAt<EventB>());
	CHECK(storage.GetSyncBuffer().TryAt<EventB>());
}

CLASS_TEST_CASE("Events that are added aren't added to the main buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();
	storage.AddEvent<EventA>();

	const ecs::EventBuffer& buffer = storage.GetMainBuffer();
	CHECK(buffer.GetAt<EventA>().GetCount() == 0);
}

CLASS_TEST_CASE("Events that are added are added to the next buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();
	storage.AddEvent<EventA>();

	const ecs::EventBuffer& buffer = storage.GetNextBuffer();
	CHECK(buffer.GetAt<EventA>().GetCount() == 1);
}

CLASS_TEST_CASE("HasEvents returns true if there are any events in the main buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();

	storage.AddEvent<EventA>();
	CHECK(!storage.HasEvents<EventA>());

	storage.FlushChanges();
	CHECK(storage.HasEvents<EventA>());
}

CLASS_TEST_CASE("GetEvents returns all events stored in the main buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();
	storage.AddEvent<EventA>();

	const auto& events = storage.GetEvents<EventA>();
	CHECK(events.m_Main.GetCount() == 0);

	storage.FlushChanges();
	CHECK(events.m_Main.GetCount() == 1);
}

CLASS_TEST_CASE("GetEvents returns all events stored in the sync buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventB>();

	ecs::EventBuffer& buffer = storage.GetSyncBuffer();
	buffer.AddEvent<EventB>();

	const auto& events = storage.GetEvents<EventB>();
	REQUIRE(events.m_Sync);
	CHECK(events.m_Sync->GetCount() == 1);
}

CLASS_TEST_CASE("GetEvents returns events from the main buffer before the sync buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventB>();

	ecs::EventBuffer& mainBuffer = storage.GetMainBuffer();
	ecs::EventBuffer& syncBuffer = storage.GetSyncBuffer();
	auto& eventA = mainBuffer.AddEvent<EventB>();
	eventA.m_Value = 1;
	auto& eventB = syncBuffer.AddEvent<EventB>();
	eventB.m_Value = 2;

	int32 value = 1;
	for (const auto& eventData : storage.GetEvents<EventB>())
		CHECK(eventData.m_Value == value++);
}

CLASS_TEST_CASE("FlushChanges moves all events from the next buffer into the main buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();

	ecs::EventBuffer& mainBuffer = storage.GetMainBuffer();
	ecs::EventBuffer& nextBuffer = storage.GetNextBuffer();
	nextBuffer.AddEvent<EventA>();
	REQUIRE(mainBuffer.GetAt<EventA>().GetCount() == 0);
	REQUIRE(nextBuffer.GetAt<EventA>().GetCount() == 1);

	storage.FlushChanges();
	CHECK(mainBuffer.GetAt<EventA>().GetCount() == 1);
	CHECK(nextBuffer.GetAt<EventA>().GetCount() == 0);
}

CLASS_TEST_CASE("FlushChanges removes all events from the main buffer and sync buffer to make room for the events in the next buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventB>();

	ecs::EventBuffer& mainBuffer = storage.GetMainBuffer();
	mainBuffer.AddEvent<EventB>();
	ecs::EventBuffer& syncBuffer = storage.GetSyncBuffer();
	syncBuffer.AddEvent<EventB>();
	REQUIRE(mainBuffer.GetAt<EventB>().GetCount() == 1);
	REQUIRE(syncBuffer.GetAt<EventB>().GetCount() == 1);

	storage.FlushChanges();
	CHECK(mainBuffer.GetAt<EventB>().GetCount() == 0);
	CHECK(syncBuffer.GetAt<EventB>().GetCount() == 0);
}

CLASS_TEST_CASE("Events that are replicated generate a sync buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventB>();

	const auto& events = storage.GetEvents<EventB>();
	CHECK(events.m_Sync);
}

CLASS_TEST_CASE("Events that aren't replicated don't generate a sync buffer.")
{
	ecs::EventStorage storage;
	storage.RegisterEvent<EventA>();

	const auto& events = storage.GetEvents<EventA>();
	CHECK(!events.m_Sync);
}