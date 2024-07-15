#include <Catch2/catch.hpp>

#include "Core/Guid.h"
#include "Core/MemBuffer.h"
#include "Math/Vector.h"

TEST_CASE("MemBuffer. Read/Write.")
{
	MemBuffer buffer;
	const str::Guid guid = str::Guid::Generate();
	const str::String string = "Hello World";

	{
		INFO("Write");

		float a = 1.f;
		double b = 2.0;
		int8 c = 3;
		int16 d = 4;
		int32 e = 5;
		uint8 f = 6;
		uint16 g = 7;
		uint32 h = 8;
		auto i = Vector2f(9);
		auto j = Vector3f(10);
		auto k = Vector4f(11);
		auto l = Vector2i(12);
		auto m = Vector3i(13);
		auto n = Vector2u(14);
		auto o = guid;
		auto p = string;

		buffer.Write(a);
		buffer.Write(b);
		buffer.Write(c);
		buffer.Write(d);
		buffer.Write(e);
		buffer.Write(f);
		buffer.Write(g);
		buffer.Write(h);
		buffer.Write(i);
		buffer.Write(j);
		buffer.Write(k);
		buffer.Write(l);
		buffer.Write(m);
		buffer.Write(n);
		buffer.Write(o);
		buffer.Write(p);
	}

	{
		INFO("Read");

		float a = 0.f;
		double b = 0.0;
		int8 c = 0;
		int16 d = 0;
		int32 e = 0;
		uint8 f = 0;
		uint16 g = 0;
		uint32 h = 0;
		Vector2f i(0.f);
		Vector3f j(0.f);
		Vector4f k(0.f);
		Vector2i l(0);
		Vector3i m(0);
		Vector2u n(0);
		str::Guid o;
		str::String p;

		buffer.Read(a);
		buffer.Read(b);
		buffer.Read(c);
		buffer.Read(d);
		buffer.Read(e);
		buffer.Read(f);
		buffer.Read(g);
		buffer.Read(h);
		buffer.Read(i);
		buffer.Read(j);
		buffer.Read(k);
		buffer.Read(l);
		buffer.Read(m);
		buffer.Read(n);
		buffer.Read(o);
		buffer.Read(p);

		CHECK(a == 1.f);
		CHECK(b == 2.0);
		CHECK(c == 3);
		CHECK(d == 4);
		CHECK(e == 5);
		CHECK(f == 6);
		CHECK(g == 7);
		CHECK(h == 8);
		CHECK(i == Vector2f(9.f));
		CHECK(j == Vector3f(10.f));
		CHECK(k == Vector4f(11.f));
		CHECK(l == Vector2i(12));
		CHECK(m == Vector3i(13));
		CHECK(n == Vector2u(14));
		CHECK(o == guid);
		CHECK(p == string);
	}
}