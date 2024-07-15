#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"
#include "ECS/QueryTypes.h"

namespace
{
	struct ComponentA : public ecs::Component<ComponentA> { };
	struct ComponentB : public ecs::Component<ComponentB> { };
	struct ComponentC : public ecs::Component<ComponentC> { };
}

TEST_CASE("ecs::query::Access. Include only ComponentA.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	using Query = ecs::query::Include<ComponentA>;

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(componentA) == true);
	CHECK(includeMask.Has(componentB) == false);
	CHECK(includeMask.Has(componentC) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(componentA) == false);
	CHECK(excludeMask.Has(componentB) == false);
	CHECK(excludeMask.Has(componentC) == false);
}

TEST_CASE("ecs::query::Access. Include both ComponentA and ComponentC.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	using Query = ecs::query::Include<ComponentA, ComponentC>;

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(componentA) == true);
	CHECK(includeMask.Has(componentB) == false);
	CHECK(includeMask.Has(componentC) == true);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(componentA) == false);
	CHECK(excludeMask.Has(componentB) == false);
	CHECK(excludeMask.Has(componentC) == false);
}

TEST_CASE("ecs::query::Access. Exclude only ComponentA.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	using Query = ecs::query::Exclude<ComponentA>;

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(componentA) == false);
	CHECK(includeMask.Has(componentB) == false);
	CHECK(includeMask.Has(componentC) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(componentA) == true);
	CHECK(excludeMask.Has(componentB) == false);
	CHECK(excludeMask.Has(componentC) == false);
}

TEST_CASE("ecs::query::Access. Exclude both ComponentA and ComponentC.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	using Query = ecs::query::Exclude<ComponentA, ComponentC>;

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(componentA) == false);
	CHECK(includeMask.Has(componentB) == false);
	CHECK(includeMask.Has(componentC) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(componentA) == true);
	CHECK(excludeMask.Has(componentB) == false);
	CHECK(excludeMask.Has(componentC) == true);
}

TEST_CASE("ecs::query::Access. Include ComponentA and Exclude ComponentC.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	using Query = ecs::query
		::Include<ComponentA>
		::Exclude<ComponentC>;

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(componentA) == true);
	CHECK(includeMask.Has(componentB) == false);
	CHECK(includeMask.Has(componentC) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(componentA) == false);
	CHECK(excludeMask.Has(componentB) == false);
	CHECK(excludeMask.Has(componentC) == true);
}
