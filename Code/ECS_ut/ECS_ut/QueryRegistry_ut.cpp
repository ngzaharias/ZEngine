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
	struct ComponentA : public ecs::Component<ComponentA> { };
	struct ComponentB : public ecs::Component<ComponentB> { };
	struct ComponentC : public ecs::Component<ComponentC> { };
}

TEST_CASE("ecs::QueryRegistry")
{
	ecs::QueryRegistry queryRegistry;
	queryRegistry.Initialise();

	{
		INFO("Include only ComponentA");

		using Query = ecs::query::Include<ComponentA>;
		CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
	}

	{
		INFO("Include both ComponentA and ComponentC");

		using Query = ecs::query::Include<ComponentA, ComponentC>;
		CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
	}

	{
		INFO("Exclude only ComponentA");

		using Query = ecs::query::Exclude<ComponentA>;
		CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
	}

	{
		INFO("Exclude both ComponentA and ComponentC");

		using Query = ecs::query::Exclude<ComponentA, ComponentC>;
		CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
	}

	{
		INFO("Include ComponentA and Exclude ComponentC");

		using Query = ecs::query
			::Include<ComponentA>
			::Exclude<ComponentC>;
		CHECK(queryRegistry.GetGroup<Query>().IsEmpty());
	}
}