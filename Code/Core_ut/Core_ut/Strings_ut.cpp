#include <Catch2/catch.hpp>

#include "Core/Address.h"

TEST_CASE("str::Address. Push/Pop.")
{
	str::Address address;
	CHECK(address.GetFull() == "");
	CHECK(address.GetLast() == "");

	address.Pop();
	CHECK(address.GetFull() == "");
	CHECK(address.GetLast() == "");

	address.Push("First");
	CHECK(address.GetFull() == "First");
	CHECK(address.GetLast() == "First");

	address.Push("Second");
	CHECK(address.GetFull() == "First/Second");
	CHECK(address.GetLast() == "Second");

	address.Push("Third");
	CHECK(address.GetFull() == "First/Second/Third");
	CHECK(address.GetLast() == "Third");

	address.Push("Fourth");
	CHECK(address.GetFull() == "First/Second/Third/Fourth");
	CHECK(address.GetLast() == "Fourth");

	address.Pop();
	CHECK(address.GetFull() == "First/Second/Third");
	CHECK(address.GetLast() == "Third");

	address.Pop();
	address.Pop();
	CHECK(address.GetFull() == "First");
	CHECK(address.GetLast() == "First");

	address.Push("Fifth");
	address.Push("Sixth");
	CHECK(address.GetFull() == "First/Fifth/Sixth");
	CHECK(address.GetLast() == "Sixth");
}