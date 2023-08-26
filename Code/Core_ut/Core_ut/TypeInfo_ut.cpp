#include <Catch2/catch.hpp>

#include <Core/TypeInfo.h>

namespace first
{
	template<typename Type, typename Tag>
	int32 MyFunction()
	{
		return ToTypeIndex<Type, Tag>();
	};
}

namespace second
{
	template<typename Type, typename Tag>
	int32 MyFunction()
	{
		return ToTypeIndex<Type, Tag>();
	};
}

TEST_CASE("TypeIndex")
{
	struct A {}; struct B {}; struct C {}; struct TagA {};
	struct D {}; struct E {}; struct F {}; struct TagB {};

	CHECK(ToTypeIndex<A, TagA>() == 0);
	CHECK(ToTypeIndex<A, TagA>() == 0);
	CHECK(ToTypeIndex<B, TagA>() == 1);
	CHECK(ToTypeIndex<C, TagA>() == 2);

	CHECK(ToTypeIndex<D, TagB>() == 0);
	CHECK(ToTypeIndex<E, TagB>() == 1);
	CHECK(ToTypeIndex<F, TagB>() == 2);

	CHECK(ToTypeIndex<D, TagA>() == 3);
	CHECK(ToTypeIndex<E, TagA>() == 4);
	CHECK(ToTypeIndex<F, TagA>() == 5);

	CHECK(ToTypeIndex<A, TagA>() == 0);

	CHECK(first::MyFunction<A, TagA>() == 0);
	CHECK(second::MyFunction<A, TagA>() == 0);
}