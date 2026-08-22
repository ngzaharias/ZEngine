#include <Catch2/catch.hpp>

#include "Core/Types.h"
#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/ComponentMask.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::ComponentMask. " name, "[ecs::ComponentMask]")

namespace
{
	struct ComponentA final : public ecs::Component { };
	struct ComponentB final : public ecs::Component { };

	const ecs::ComponentId s_ComponentAId = ToTypeId<ComponentA, ecs::ComponentTag>();
	const ecs::ComponentId s_ComponentBId = ToTypeId<ComponentB, ecs::ComponentTag>();

	constexpr int32 s_IndexA = 137;
	constexpr int32 s_IndexB = 200;
	static_assert(s_IndexA < ecs::COMPONENTS_MAX);
	static_assert(s_IndexB < ecs::COMPONENTS_MAX);
}

CLASS_TEST_CASE("operator<(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK(maskA.operator<(maskB));
	CHECK_FALSE(maskB.operator<(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK_FALSE(maskA.operator<(maskB));
	CHECK_FALSE(maskB.operator<(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK_FALSE(maskA.operator<(maskB));
	CHECK_FALSE(maskB.operator<(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK_FALSE(maskA.operator<(maskB));
	CHECK_FALSE(maskB.operator<(maskA));
}

CLASS_TEST_CASE("operator<=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK(maskA.operator<=(maskB));
	CHECK_FALSE(maskB.operator<=(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK(maskA.operator<=(maskB));
	CHECK(maskB.operator<=(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK(maskA.operator<=(maskB));
	CHECK(maskB.operator<=(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK(maskA.operator<=(maskB));
	CHECK(maskB.operator<=(maskA));
}

CLASS_TEST_CASE("operator>(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK_FALSE(maskA.operator>(maskB));
	CHECK(maskB.operator>(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK_FALSE(maskA.operator>(maskB));
	CHECK_FALSE(maskB.operator>(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK_FALSE(maskA.operator>(maskB));
	CHECK_FALSE(maskB.operator>(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK_FALSE(maskA.operator>(maskB));
	CHECK_FALSE(maskB.operator>(maskA));
}

CLASS_TEST_CASE("operator>=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK_FALSE(maskA.operator>=(maskB));
	CHECK(maskB.operator>=(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK(maskA.operator>=(maskB));
	CHECK(maskB.operator>=(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK(maskA.operator>=(maskB));
	CHECK(maskB.operator>=(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK(maskA.operator>=(maskB));
	CHECK(maskB.operator>=(maskA));
}

CLASS_TEST_CASE("operator==(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK_FALSE(maskA.operator==(maskB));
	CHECK_FALSE(maskB.operator==(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK(maskA.operator==(maskB));
	CHECK(maskB.operator==(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK(maskA.operator==(maskB));
	CHECK(maskB.operator==(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK(maskA.operator==(maskB));
	CHECK(maskB.operator==(maskA));
}

CLASS_TEST_CASE("operator!=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexB);
	CHECK(maskA.operator!=(maskB));
	CHECK(maskB.operator!=(maskA));

	maskA.RaiseAll();
	maskB.RaiseAll();
	CHECK_FALSE(maskA.operator!=(maskB));
	CHECK_FALSE(maskB.operator!=(maskA));

	maskA.ClearAll();
	maskB.ClearAll();
	CHECK_FALSE(maskA.operator!=(maskB));
	CHECK_FALSE(maskB.operator!=(maskA));

	maskA.Raise(s_IndexA);
	maskB.Raise(s_IndexA);
	CHECK_FALSE(maskA.operator!=(maskB));
	CHECK_FALSE(maskB.operator!=(maskA));
}

CLASS_TEST_CASE("operator|(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC = maskA | maskB;
	CHECK(maskC.Has(0));
	CHECK(maskC.Has(1));
	CHECK(maskC.Has(2));
}

CLASS_TEST_CASE("operator&(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC = maskA & maskB;
	CHECK(!maskC.Has(0));
	CHECK(maskC.Has(1));
	CHECK(!maskC.Has(2));
}

CLASS_TEST_CASE("operator^(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC = maskA ^ maskB;
	CHECK(maskC.Has(0));
	CHECK(!maskC.Has(1));
	CHECK(maskC.Has(2));
}

CLASS_TEST_CASE("operator~().")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;

	constexpr int32 counH = ecs::COMPONENTS_MAX / 2;
	constexpr int32 countF = ecs::COMPONENTS_MAX;
	for (int32 i = 0; i < counH; ++i)
		maskA.Raise(i);
	
	maskB = ~maskA;

	for (int32 i = 0; i < counH; ++i)
		CHECK(!maskB.Has(i));
	for (int32 i = counH; i < countF; ++i)
		CHECK(maskB.Has(i));
}

CLASS_TEST_CASE("operator|=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0);
	maskA.Raise(1);
	maskB.Raise(1);
	maskB.Raise(2);

	maskC = maskA;
	maskC |= maskB;
	CHECK(maskC.Has(0));
	CHECK(maskC.Has(1));
	CHECK(maskC.Has(2));
}

CLASS_TEST_CASE("operator&=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0);
	maskA.Raise(1);
	maskB.Raise(1);
	maskB.Raise(2);

	maskC = maskA;
	maskC &= maskB;
	CHECK(!maskC.Has(0));
	CHECK(maskC.Has(1));
	CHECK(!maskC.Has(2));
}

CLASS_TEST_CASE("operator^=(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0);
	maskA.Raise(1);
	maskB.Raise(1);
	maskB.Raise(2);

	maskC = maskA;
	maskC ^= maskB;
	CHECK(maskC.Has(0));
	CHECK(!maskC.Has(1));
	CHECK(maskC.Has(2));
}

CLASS_TEST_CASE("Clear(int32).")
{
	ecs::ComponentMask mask;
	mask.Raise(s_IndexA);
	CHECK(mask.Has(s_IndexA));
	mask.Clear(s_IndexA);
	CHECK(!mask.Has(s_IndexA));
}

CLASS_TEST_CASE("Clear(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC = maskA;
	maskC.Clear(maskB);
	CHECK(maskC.Has(0));
	CHECK(!maskC.Has(1));
	CHECK(!maskC.Has(2));
}

CLASS_TEST_CASE("ClearAll().")
{
	ecs::ComponentMask mask;
	mask.RaiseAll();
	CHECK(mask.HasAll());
	mask.ClearAll();
	CHECK(mask.HasNone());
}

CLASS_TEST_CASE("Raise(int32).")
{
	ecs::ComponentMask mask;
	CHECK(!mask.Has(s_IndexA));
	mask.Raise(s_IndexA);
	CHECK(mask.Has(s_IndexA));
}

CLASS_TEST_CASE("Raise(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC = maskA;
	maskC.Raise(maskB);
	CHECK(maskC.Has(0));
	CHECK(maskC.Has(1));
	CHECK(maskC.Has(2));
}

CLASS_TEST_CASE("RaiseAll().")
{
	ecs::ComponentMask mask;
	CHECK(mask.HasNone());
	mask.RaiseAll();
	CHECK(mask.HasAll());
}

CLASS_TEST_CASE("Has(int32).")
{
	ecs::ComponentMask mask;
	CHECK(!mask.Has(s_IndexA));
	mask.Raise(s_IndexA);
	CHECK(mask.Has(s_IndexA));
	mask.Clear(s_IndexA);
	CHECK(!mask.Has(s_IndexA));
}

CLASS_TEST_CASE("HasAll().")
{
	ecs::ComponentMask mask;
	CHECK(!mask.HasAll());
	mask.Raise(s_IndexA);
	CHECK(!mask.HasAll());
	mask.RaiseAll();
	CHECK(mask.HasAll());
	mask.Clear(s_IndexA);
	CHECK(!mask.HasAll());
	mask.ClearAll();
	CHECK(!mask.HasAll());
}

CLASS_TEST_CASE("HasAll(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC.Raise(0); maskC.Raise(1);
	CHECK(!maskA.HasAll(maskB));
	CHECK(maskA.HasAll(maskC));
}

CLASS_TEST_CASE("HasAny().")
{
	ecs::ComponentMask mask;
	CHECK(!mask.HasAny());
	mask.Raise(s_IndexA);
	CHECK(mask.HasAny());
	mask.RaiseAll();
	CHECK(mask.HasAny());
	mask.Clear(s_IndexA);
	CHECK(mask.HasAny());
	mask.ClearAll();
	CHECK(!mask.HasAny());
}

CLASS_TEST_CASE("HasAny(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1); maskA.Raise(2);
	maskB.Raise(2); maskB.Raise(3); maskB.Raise(4);

	maskC.Raise(0); maskC.Raise(1); maskC.Raise(2); maskC.Raise(3);
	CHECK(maskC.HasAll(maskA));
	CHECK(!maskC.HasAll(maskB));
}

CLASS_TEST_CASE("HasNone().")
{
	ecs::ComponentMask mask;
	CHECK(mask.HasNone());
	mask.Raise(s_IndexA);
	CHECK(!mask.HasNone());
	mask.RaiseAll();
	CHECK(!mask.HasNone());
	mask.Clear(s_IndexA);
	CHECK(!mask.HasNone());
	mask.ClearAll();
	CHECK(mask.HasNone());
}

CLASS_TEST_CASE("HasNone(ComponentMask).")
{
	ecs::ComponentMask maskA;
	ecs::ComponentMask maskB;
	ecs::ComponentMask maskC;
	maskA.Raise(0); maskA.Raise(1);
	maskB.Raise(1); maskB.Raise(2);

	maskC.Raise(2);
	CHECK(!maskA.HasNone(maskB));
	CHECK(maskA.HasNone(maskC));
}

CLASS_TEST_CASE("Range-based for loop.")
{
	ecs::ComponentMask mask;
	mask.Raise(6); mask.Raise(7);
	mask.Raise(88); mask.Raise(102);

	int32 i = 0;
	ecs::ComponentId checks[] = { 6, 7, 88, 102 };
	for (ecs::ComponentId value : mask)
		CHECK(value == checks[i++]);
	CHECK(i == 4);
}

CLASS_TEST_CASE("ToComponentMask with no components.")
{
	auto componentMask = ecs::ToComponentMask<>();
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("ToComponentMask with a single component.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("ToComponentMask with multiple components.")
{
	auto componentMask = ecs::ToComponentMask<ComponentA, ComponentB>();
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

CLASS_TEST_CASE("ToComponentMask with a TypeList of no components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<>{});
	CHECK(componentMask.Has(s_ComponentAId) == false);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("ToComponentMask with a TypeList of one component.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == false);
}

CLASS_TEST_CASE("ToComponentMask with a TypeList of multiple components.")
{
	auto componentMask = ecs::ToComponentMask(TypeList<ComponentA, ComponentB>{});
	CHECK(componentMask.Has(s_ComponentAId) == true);
	CHECK(componentMask.Has(s_ComponentBId) == true);
}

CLASS_TEST_CASE("ToComponentMask has const and non-const result in the same mask.")
{
	auto componentMaskA = ecs::ToComponentMask<ComponentA>();
	auto componentMaskB = ecs::ToComponentMask<const ComponentA>();
	CHECK(componentMaskA == componentMaskB);
}
