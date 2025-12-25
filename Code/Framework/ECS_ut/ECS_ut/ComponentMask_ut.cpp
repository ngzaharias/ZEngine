#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { };
	struct ComponentB final : public ecs::Component<ComponentB> { };

	const ecs::ComponentId s_ComponentAId = ToTypeId<ComponentA, ecs::ComponentTag>();
	const ecs::ComponentId s_ComponentBId = ToTypeId<ComponentB, ecs::ComponentTag>();
}

TEST_CASE("ecs::ComponentMask. No components.")
{
	auto componentMask = ecs::ToComponentMask<>();
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

TEST_CASE("ecs::ComponentMask. Single component.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

TEST_CASE("ecs::ComponentMask. Multiple components.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA, ComponentB>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

TEST_CASE("ecs::ComponentMask. TypeList with no components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<>{});
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

TEST_CASE("ecs::ComponentMask. TypeList with one component.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

TEST_CASE("ecs::ComponentMask. TypeList with multiple components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA, ComponentB>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

TEST_CASE("ecs::ComponentMask. Const and non-const result in the same mask.")
{
	auto componentMaskA = ecs::ToComponentMask<ComponentA>();
	auto componentMaskB = ecs::ToComponentMask<const ComponentA>();
	CHECK(componentMaskA.HasAll(componentMaskB));
}
