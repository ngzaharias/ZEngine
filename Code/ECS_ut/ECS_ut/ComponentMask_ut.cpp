#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"

namespace
{
	struct ComponentA : public ecs::Component<ComponentA> { };
	struct ComponentB : public ecs::Component<ComponentB> { };
	struct ComponentC : public ecs::Component<ComponentC> { };
}

TEST_CASE("ecs::ComponentMask. No Components.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask<>();
	CHECK(componentMask.Has(componentA) == false);
	CHECK(componentMask.Has(componentB) == false);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. Single Component.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask<ComponentA>();
	CHECK(componentMask.Has(componentA) == true);
	CHECK(componentMask.Has(componentB) == false);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. Multiple Components.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask<ComponentA, ComponentB>();
	CHECK(componentMask.Has(componentA) == true);
	CHECK(componentMask.Has(componentB) == true);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. No Component TypeList.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask(TypeList<>{});
	CHECK(componentMask.Has(componentA) == false);
	CHECK(componentMask.Has(componentB) == false);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. Single Component TypeList.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA>{});
	CHECK(componentMask.Has(componentA) == true);
	CHECK(componentMask.Has(componentB) == false);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. Multiple Component TypeList.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA, ComponentB>{});
	CHECK(componentMask.Has(componentA) == true);
	CHECK(componentMask.Has(componentB) == true);
	CHECK(componentMask.Has(componentC) == false);
}

TEST_CASE("ecs::ComponentMask. Non-const == Const.")
{
	const int32 componentA = ToTypeIndex<ComponentA, ecs::ComponentTag>();
	const int32 componentB = ToTypeIndex<ComponentB, ecs::ComponentTag>();
	const int32 componentC = ToTypeIndex<ComponentC, ecs::ComponentTag>();

	auto componentMaskA = ecs::ToComponentMask<ComponentA>();
	auto componentMaskB = ecs::ToComponentMask<const ComponentA>();
	CHECK(componentMaskA.HasAll(componentMaskB));
}
