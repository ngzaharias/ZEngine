#include "CorePCH.h"

#include "Core/Guid.h"
#include "Core/MemBuffer.h"
#include "Math/Vector.h"

namespace
{
	struct Trivial
	{
		bool m_Data = false;
	};

	struct NonTrivial
	{
		str::String m_Data = {};
	};

	static_assert(std::is_trivially_copyable<Trivial>::value, "");
	static_assert(!std::is_trivially_copyable<NonTrivial>::value, "");
}

template<>
void MemBuffer::Read(NonTrivial& value) const
{
	Read(value.m_Data);
}

template<>
void MemBuffer::Write(const NonTrivial& value)
{
	Write(value.m_Data);
}

TEST_CASE("MemBuffer. bool.")
{
	MemBuffer buffer;
	{
		const bool object = true;
		buffer.Write(object);
	}
	{
		bool object = false;
		buffer.Read(object);
		CHECK(object == true);
	}
}

TEST_CASE("MemBuffer. float.")
{
	MemBuffer buffer;
	{
		const float object = 1.f;
		buffer.Write(object);
	}
	{
		float object = 0.f;
		buffer.Read(object);
		CHECK(object == 1.f);
	}
}

TEST_CASE("MemBuffer. double.")
{
	MemBuffer buffer;
	{
		const double object = 1.0;
		buffer.Write(object);
	}
	{
		double object = 0.0;
		buffer.Read(object);
		CHECK(object == 1.0);
	}
}

TEST_CASE("MemBuffer. int8.")
{
	MemBuffer buffer;
	{
		const int8 object = 1;
		buffer.Write(object);
	}
	{
		int8 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. int16.")
{
	MemBuffer buffer;
	{
		const int16 object = 1;
		buffer.Write(object);
	}
	{
		int16 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. int32.")
{
	MemBuffer buffer;
	{
		const int32 object = 1;
		buffer.Write(object);
	}
	{
		int32 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. uint8.")
{
	MemBuffer buffer;
	{
		const uint8 object = 1;
		buffer.Write(object);
	}
	{
		uint8 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. uint16.")
{
	MemBuffer buffer;
	{
		const uint16 object = 1;
		buffer.Write(object);
	}
	{
		uint16 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. uint32.")
{
	MemBuffer buffer;
	{
		const uint32 object = 1;
		buffer.Write(object);
	}
	{
		uint32 object = 0;
		buffer.Read(object);
		CHECK(object == 1);
	}
}

TEST_CASE("MemBuffer. Vector2f.")
{
	MemBuffer buffer;
	{
		const Vector2f object = Vector2f(1.f);
		buffer.Write(object);
	}
	{
		Vector2f object = Vector2f(0.f);
		buffer.Read(object);
		CHECK(object == Vector2f(1.f));
	}
}

TEST_CASE("MemBuffer. Vector3f.")
{
	MemBuffer buffer;
	{
		const Vector3f object = Vector3f(1.f);
		buffer.Write(object);
	}
	{
		Vector3f object = Vector3f(0.f);
		buffer.Read(object);
		CHECK(object == Vector3f(1.f));
	}
}

TEST_CASE("MemBuffer. Vector4f.")
{
	MemBuffer buffer;
	{
		const Vector4f object = Vector4f(1.f);
		buffer.Write(object);
	}
	{
		Vector4f object = Vector4f(0.f);
		buffer.Read(object);
		CHECK(object == Vector4f(1.f));
	}
}

TEST_CASE("MemBuffer. Vector2i.")
{
	MemBuffer buffer;
	{
		const Vector2i object = Vector2i(1);
		buffer.Write(object);
	}
	{
		Vector2i object = Vector2i(0);
		buffer.Read(object);
		CHECK(object == Vector2i(1));
	}
}

TEST_CASE("MemBuffer. Vector3i.")
{
	MemBuffer buffer;
	{
		const Vector3i object = Vector3i(1);
		buffer.Write(object);
	}
	{
		Vector3i object = Vector3i(0);
		buffer.Read(object);
		CHECK(object == Vector3i(1));
	}
}

TEST_CASE("MemBuffer. Vector2u.")
{
	MemBuffer buffer;
	{
		const Vector2u object = Vector2u(1);
		buffer.Write(object);
	}
	{
		Vector2u object = Vector2u(0);
		buffer.Read(object);
		CHECK(object == Vector2u(1));
	}
}

TEST_CASE("MemBuffer. Guid.")
{
	const str::Guid guidA = GUID("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	const str::Guid guidB = GUID("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	MemBuffer buffer;
	{
		const str::Guid object = guidA;
		buffer.Write(object);
	}
	{
		str::Guid object = guidB;
		buffer.Read(object);
		CHECK(object == guidA);
	}
}

TEST_CASE("MemBuffer. String.")
{
	MemBuffer buffer;
	{
		const str::String object = "Foo";
		buffer.Write(object);
	}
	{
		str::String object = "Bar";
		buffer.Read(object);
		CHECK(object == "Foo");
	}
}

TEST_CASE("MemBuffer. Trivial.")
{
	MemBuffer buffer;

	// write
	{
		Trivial object = { true };
		buffer.Write(object);
	}

	// read
	{
		Trivial object = { false };
		buffer.Read(object);
		CHECK(object.m_Data == true);
	}
}

TEST_CASE("MemBuffer. NonTrivial.")
{
	MemBuffer buffer;

	// write
	{
		NonTrivial object = { "Foo" };
		buffer.Write(object);
	}

	// read
	{
		NonTrivial object = { "Bar" };
		buffer.Read(object);
		CHECK(object.m_Data == "Foo");
	}
}