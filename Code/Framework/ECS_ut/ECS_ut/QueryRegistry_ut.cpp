#include <Catch2/catch.hpp>

#include "Core/Algorithms.h"
#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryRegistry.h"

class GameTime {};

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { };
	struct ComponentB final : public ecs::Component<ComponentB> { };
}

TEST_CASE("ecs::QueryRegistry. Include only ComponentA.")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	using Query = ecs::query::Include<ComponentA>;
	CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
}

TEST_CASE("ecs::QueryRegistry. Include both ComponentA and ComponentC.")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	using Query = ecs::query::Include<ComponentA, ComponentB>;
	CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
}

TEST_CASE("ecs::QueryRegistry. Exclude only ComponentA.")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	using Query = ecs::query::Exclude<ComponentA>;
	CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
}

TEST_CASE("ecs::QueryRegistry. Exclude both ComponentA and ComponentB.")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	using Query = ecs::query::Exclude<ComponentA, ComponentB>;
	CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
}

TEST_CASE("ecs::QueryRegistry. Include ComponentA and Exclude ComponentB.")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	using Query = ecs::query
		::Include<ComponentA>
		::Exclude<ComponentB>;
	CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
}
