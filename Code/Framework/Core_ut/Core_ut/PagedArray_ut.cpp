#include "CorePCH.h"

#include "Core/Algorithms.h"
#include "Core/PagedArray.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("PagedArray. " name, "[PagedArray]")

namespace
{
	struct MyStruct
	{
	};
}

CLASS_TEST_CASE("Insert.")
{
	PagedArray<float, 2> myArray;
	myArray.Insert(8, 1337.f);
}