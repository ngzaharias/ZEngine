#include "CorePCH.h"

#include "Core/TypeInfo.h"

#define HELPER_TEST_CASE(name) TEST_CASE("TypeInfo. " name, "[TypeInfo]")

namespace first
{
	template<typename Type, typename Tag>
	int32 MyFunction()
	{
		return ToTypeId<Type, Tag>();
	};
}

namespace second
{
	template<typename Type, typename Tag>
	TypeId MyFunction()
	{
		return ToTypeId<Type, Tag>();
	};
}

HELPER_TEST_CASE("ToTypeId.")
{
	struct A {}; struct B {}; struct C {}; struct TagA {};
	struct D {}; struct E {}; struct F {}; struct TagB {};

	CHECK(ToTypeId<A, TagA>() == 0);
	CHECK(ToTypeId<A, TagA>() == 0);
	CHECK(ToTypeId<B, TagA>() == 1);
	CHECK(ToTypeId<C, TagA>() == 2);

	CHECK(ToTypeId<D, TagB>() == 0);
	CHECK(ToTypeId<E, TagB>() == 1);
	CHECK(ToTypeId<F, TagB>() == 2);

	CHECK(ToTypeId<D, TagA>() == 3);
	CHECK(ToTypeId<E, TagA>() == 4);
	CHECK(ToTypeId<F, TagA>() == 5);

	CHECK(ToTypeId<A, TagA>() == 0);

	CHECK(first::MyFunction<A, TagA>() == 0);
	CHECK(second::MyFunction<A, TagA>() == 0);
}