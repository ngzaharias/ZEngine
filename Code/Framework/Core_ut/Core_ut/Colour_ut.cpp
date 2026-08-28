#include "CorePCH.h"

#include "Core/Colour.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Colour. " name, "[Colour]")

std::ostream& operator<<(std::ostream& os, const Colour& value)
{
	return os << "{ "
		<< "r: " << value.r << ", "
		<< "g: " << value.g << ", "
		<< "b: " << value.b << ", "
		<< "a: " << value.a << " }";
}

CLASS_TEST_CASE("Alignment.")
{
	CHECK(alignof(Colour) == 4);
}

CLASS_TEST_CASE("Colour().")
{
	constexpr Colour colour;
	CHECK(colour.r == 0.f);
	CHECK(colour.g == 0.f);
	CHECK(colour.b == 0.f);
	CHECK(colour.a == 0.f);
}

CLASS_TEST_CASE("Colour(int32).")
{
	constexpr Colour colour(0xFFFFFF);
	CHECK(colour.r == 1.f);
	CHECK(colour.g == 1.f);
	CHECK(colour.b == 1.f);
	CHECK(colour.a == 1.f);
}

CLASS_TEST_CASE("Colour(uint32).")
{
	constexpr Colour colour(0xFFFFFF00);
	CHECK(colour.r == 1.f);
	CHECK(colour.g == 1.f);
	CHECK(colour.b == 1.f);
	CHECK(colour.a == 0.f);
}

CLASS_TEST_CASE("Colour(float, float).")
{
	constexpr Colour colour(2.f, 3.f);
	CHECK(colour.r == 2.f);
	CHECK(colour.g == 2.f);
	CHECK(colour.b == 2.f);
	CHECK(colour.a == 3.f);
}

CLASS_TEST_CASE("Colour(Vector3f, float).")
{
	constexpr Colour colour(Vector3f(1.f, 2.f, 3.f), 4.f);
	CHECK(colour.r == 1.f);
	CHECK(colour.g == 2.f);
	CHECK(colour.b == 3.f);
	CHECK(colour.a == 4.f);
}

CLASS_TEST_CASE("Colour(Vector4f).")
{
	constexpr Colour colour(Vector4f(1.f, 2.f, 3.f, 4.f));
	CHECK(colour.r == 1.f);
	CHECK(colour.g == 2.f);
	CHECK(colour.b == 3.f);
	CHECK(colour.a == 4.f);
}

CLASS_TEST_CASE("operator==(Colour).")
{
	CHECK(Colour(0.f, 0.f, 0.f, 0.f) == Colour(0.f, 0.f, 0.f, 0.f));
	CHECK_FALSE(Colour(0.f, 0.f, 0.f, 0.f) == Colour(1.f, 1.f, 1.f, 1.f));
}

CLASS_TEST_CASE("operator!=(Colour).")
{
	CHECK(Colour(0.f, 0.f, 0.f, 0.f) != Colour(1.f, 1.f, 1.f, 1.f));
	CHECK_FALSE(Colour(0.f, 0.f, 0.f, 0.f) != Colour(0.f, 0.f, 0.f, 0.f));
}

CLASS_TEST_CASE("operator*(float).")
{
	Colour colour = Colour(2.f, 2.f, 2.f, 2.f) * 3.f;
	CHECK(colour == Colour(6.f, 6.f, 6.f, 6.f));
}

CLASS_TEST_CASE("operator/(float).")
{
	Colour colour = Colour(6.f, 6.f, 6.f, 6.f) / 3.f;
	CHECK(colour == Colour(2.f, 2.f, 2.f, 2.f));
}

CLASS_TEST_CASE("operator*=(float).")
{
	Colour colour = Colour(2.f, 2.f, 2.f, 2.f);
	colour *= 3.f;
	CHECK(colour == Colour(6.f, 6.f, 6.f, 6.f));
}

CLASS_TEST_CASE("operator/=(float).")
{
	Colour colour = Colour(6.f, 6.f, 6.f, 6.f);
	colour /= 3.f;
	CHECK(colour == Colour(2.f, 2.f, 2.f, 2.f));
}

CLASS_TEST_CASE("Black.")
{
	CHECK(Colour::Black.r == 0.f);
	CHECK(Colour::Black.g == 0.f);
	CHECK(Colour::Black.b == 0.f);
	CHECK(Colour::Black.a == 1.f);
}

CLASS_TEST_CASE("Blue.")
{
	CHECK(Colour::Blue.r == 0.f);
	CHECK(Colour::Blue.g == 0.f);
	CHECK(Colour::Blue.b == 1.f);
	CHECK(Colour::Blue.a == 1.f);
}

CLASS_TEST_CASE("Cyan.")
{
	CHECK(Colour::Cyan.r == 0.f);
	CHECK(Colour::Cyan.g == 1.f);
	CHECK(Colour::Cyan.b == 1.f);
	CHECK(Colour::Cyan.a == 1.f);
}

CLASS_TEST_CASE("Gray.")
{
	CHECK(Colour::Gray.r == Approx(0.501961f));
	CHECK(Colour::Gray.g == Approx(0.501961f));
	CHECK(Colour::Gray.b == Approx(0.501961f));
	CHECK(Colour::Gray.a == 1.f);
}

CLASS_TEST_CASE("Green.")
{
	CHECK(Colour::Green.r == 0.f);
	CHECK(Colour::Green.g == 1.f);
	CHECK(Colour::Green.b == 0.f);
	CHECK(Colour::Green.a == 1.f);
}

CLASS_TEST_CASE("Magenta.")
{
	CHECK(Colour::Magenta.r == 1.f);
	CHECK(Colour::Magenta.g == 0.f);
	CHECK(Colour::Magenta.b == 1.f);
	CHECK(Colour::Magenta.a == 1.f);
}

CLASS_TEST_CASE("Red.")
{
	CHECK(Colour::Red.r == 1.f);
	CHECK(Colour::Red.g == 0.f);
	CHECK(Colour::Red.b == 0.f);
	CHECK(Colour::Red.a == 1.f);
}

CLASS_TEST_CASE("White.")
{
	CHECK(Colour::White.r == 1.f);
	CHECK(Colour::White.g == 1.f);
	CHECK(Colour::White.b == 1.f);
	CHECK(Colour::White.a == 1.f);
}

CLASS_TEST_CASE("Yellow.")
{
	CHECK(Colour::Yellow.r == 1.f);
	CHECK(Colour::Yellow.g == 1.f);
	CHECK(Colour::Yellow.b == 0.f);
	CHECK(Colour::Yellow.a == 1.f);
}