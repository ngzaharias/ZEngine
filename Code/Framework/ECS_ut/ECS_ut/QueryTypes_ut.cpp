#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"
#include "ECS/QueryTypes.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::QueryAccess. " name, "[ecs::QueryAccess]")

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { };
	struct ComponentB final : public ecs::Component<ComponentB> { };

	const ecs::ComponentId s_ComponentAId = ToTypeId<ComponentA, ecs::ComponentTag>();
	const ecs::ComponentId s_ComponentBId = ToTypeId<ComponentB, ecs::ComponentTag>();
}

CLASS_TEST_CASE("Added only ComponentA.")
{
	using Query = ecs::query::Added<ComponentA>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == true);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Added both ComponentAand ComponentB.")
{
	using Query = ecs::query::Added<ComponentA, ComponentB>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == true);
	CHECK(addedMask.Has(s_ComponentBId) == true);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Removed only ComponentA.")
{
	using Query = ecs::query::Removed<ComponentA>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == true);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Removed both ComponentAand ComponentB.")
{
	using Query = ecs::query::Removed<ComponentA, ComponentB>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == true);
	CHECK(removedMask.Has(s_ComponentBId) == true);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Updated only ComponentA.")
{
	using Query = ecs::query::Updated<ComponentA>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == true);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Updated both ComponentAand ComponentB.")
{
	using Query = ecs::query::Updated<ComponentA, ComponentB>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == true);
	CHECK(updatedMask.Has(s_ComponentBId) == true);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Include only ComponentA.")
{
	using Query = ecs::query::Include<ComponentA>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == true);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Include both ComponentAand ComponentB.")
{
	using Query = ecs::query::Include<ComponentA, ComponentB>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == true);
	CHECK(includeMask.Has(s_ComponentBId) == true);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == false);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Exclude only ComponentA.")
{
	using Query = ecs::query::Exclude<ComponentA>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == true);
	CHECK(excludeMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Exclude both ComponentA and ComponentC.")
{
	using Query = ecs::query::Exclude<ComponentA, ComponentB>;

	const auto addedMask = ecs::ToComponentMask(ecs::query::AddedAccess<Query>{});
	CHECK(addedMask.Has(s_ComponentAId) == false);
	CHECK(addedMask.Has(s_ComponentBId) == false);

	const auto removedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<Query>{});
	CHECK(removedMask.Has(s_ComponentAId) == false);
	CHECK(removedMask.Has(s_ComponentBId) == false);

	const auto updatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<Query>{});
	CHECK(updatedMask.Has(s_ComponentAId) == false);
	CHECK(updatedMask.Has(s_ComponentBId) == false);

	const auto includeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<Query>{});
	CHECK(includeMask.Has(s_ComponentAId) == false);
	CHECK(includeMask.Has(s_ComponentBId) == false);

	const auto excludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<Query>{});
	CHECK(excludeMask.Has(s_ComponentAId) == true);
	CHECK(excludeMask.Has(s_ComponentBId) == true);
}
