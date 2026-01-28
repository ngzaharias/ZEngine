#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::ComponentMask. " name, "[ecs::ComponentMask]")

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { };
	struct ComponentB final : public ecs::Component<ComponentB> { };

	const ecs::ComponentId s_ComponentAId = ToTypeId<ComponentA, ecs::ComponentTag>();
	const ecs::ComponentId s_ComponentBId = ToTypeId<ComponentB, ecs::ComponentTag>();
}

CLASS_TEST_CASE("No components.")
{
	auto componentMask = ecs::ToComponentMask<>();
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Single component.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("Multiple components.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA, ComponentB>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

CLASS_TEST_CASE("TypeList with no components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<>{});
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("TypeList with one component.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("TypeList with multiple components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA, ComponentB>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

CLASS_TEST_CASE("Const and non-const result in the same mask.")
{
	auto componentMaskA = ecs::ToComponentMask<ComponentA>();
	auto componentMaskB = ecs::ToComponentMask<const ComponentA>();
	CHECK(componentMaskA.HasAll(componentMaskB));
}
