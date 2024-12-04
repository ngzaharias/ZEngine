#include <Catch2/catch.hpp>

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Optional.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Engine/Visitor.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include <format>

namespace
{
	const str::Guid& strGuidA = GUID("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	const str::Guid& strGuidB = GUID("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	constexpr str::StringView strBool = "Bool";
	constexpr str::StringView strFloat = "Float";
	constexpr str::StringView strInt32 = "Int32";
	constexpr str::StringView strMyKey = "MyKey";

	enum class EInt8 : int8 { Min = -INT8_MAX, Max = BIT32(6) };
	enum class EInt16 : int16 { Min = -INT16_MAX, Max = BIT32(14) };
	enum class EInt32 : int32 { Min = -INT32_MAX, Max = BIT32(30) };
	enum class EInt64 : int64 { Min = -INT64_MAX, Max = BIT64(62) };

	enum class EUInt8 : uint8 { Min = 0, Max = BIT32(6) };
	enum class EUInt16 : uint16 { Min = 0, Max = BIT32(14) };
	enum class EUInt32 : uint32 { Min = 0, Max = BIT32(30) };
	enum class EUInt64 : uint64 { Min = 0, Max = BIT64(62) };

	struct Struct
	{
		bool m_Bool = false;
		float m_Float = 0.f;
		int32 m_Int32 = 0;
	};
}

template<>
void eng::Visitor::ReadCustom(Struct& value) const
{
	Read(strBool, value.m_Bool, false);
	Read(strFloat, value.m_Float, 0.f);
	Read(strInt32, value.m_Int32, 0);
}

template<>
void eng::Visitor::WriteCustom(const Struct& value)
{
	Write(strBool, value.m_Bool);
	Write(strFloat, value.m_Float);
	Write(strInt32, value.m_Int32);
}

CATCH_REGISTER_ENUM(EInt8, EInt8::Min, EInt8::Max);
CATCH_REGISTER_ENUM(EInt16, EInt16::Min, EInt16::Max);
CATCH_REGISTER_ENUM(EInt32, EInt32::Min, EInt32::Max);
CATCH_REGISTER_ENUM(EUInt8, EUInt8::Min, EUInt8::Max);
CATCH_REGISTER_ENUM(EUInt16, EUInt16::Min, EUInt16::Max);
CATCH_REGISTER_ENUM(EUInt32, EUInt32::Min, EUInt32::Max);

TEST_CASE("eng::Visitor::Bool")
{
	{
		INFO("Write");
		bool myBool = true;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myBool);

		str::String string = visitor;
		CHECK(string == "MyKey = true");
	}

	{
		INFO("Read");
		bool myBool = false;
		eng::Visitor visitor = str::StringView("MyKey = true");
		visitor.Read(strMyKey, myBool, false);

		CHECK(myBool == true);
	}

	{
		INFO("Read - Default");
		bool myBool = false;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myBool, true);

		CHECK(myBool == true);
	}
}

TEST_CASE("eng::Visitor::Float")
{
	{
		INFO("Write");
		float myFloat = 2.f;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myFloat);

		str::String string = visitor;
		CHECK(string == "MyKey = 2.0");
	}

	{
		INFO("Read");
		float myFloat = 1.f;
		eng::Visitor visitor = str::StringView("MyKey = 2.0");
		visitor.Read(strMyKey, myFloat, 1.f);

		CHECK(myFloat == 2.f);
	}

	{
		INFO("Read - Default");
		float myFloat = 1.f;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myFloat, 2.f);

		CHECK(myFloat == 2.f);
	}
}

TEST_CASE("eng::Visitor::Double")
{
	{
		INFO("Write");
		double myDouble = 2.0;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myDouble);

		str::String string = visitor;
		CHECK(string == "MyKey = 2.0");
	}

	{
		INFO("Read");
		double myDouble = 1.0;
		eng::Visitor visitor = str::StringView("MyKey = 2.0");
		visitor.Read(strMyKey, myDouble, 1.0);

		CHECK(myDouble == 2.0);
	}

	{
		INFO("Read - Default");
		double myDouble = 1.0;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myDouble, 2.0);

		CHECK(myDouble == 2.0);
	}
}

TEST_CASE("eng::Visitor::Int8")
{
	{
		INFO("Write");
		int8 myInt8 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myInt8);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		int8 myInt8 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myInt8, int8(1));

		CHECK(myInt8 == 2);
	}

	{
		INFO("Read - Default");
		int8 myInt8 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myInt8, int8(2));

		CHECK(myInt8 == 2);
	}
}

TEST_CASE("eng::Visitor::Int16")
{
	{
		INFO("Write");
		int16 myInt16 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myInt16);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		int16 myInt16 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myInt16, int16(1));

		CHECK(myInt16 == 2);
	}

	{
		INFO("Read - Default");
		int16 myInt16 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myInt16, int16(2));

		CHECK(myInt16 == 2);
	}
}

TEST_CASE("eng::Visitor::Int32")
{
	{
		INFO("Write");
		int32 myInt32 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myInt32);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		int32 myInt32 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myInt32, 1);

		CHECK(myInt32 == 2);
	}

	{
		INFO("Read - Default");
		int32 myInt32 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myInt32, 2);

		CHECK(myInt32 == 2);
	}
}

TEST_CASE("eng::Visitor::Int64")
{
	{
		INFO("Write");
		int64 myInt64 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myInt64);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		int64 myInt64 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myInt64, int64(1));

		CHECK(myInt64 == 2);
	}

	{
		INFO("Read - Default");
		int64 myInt64 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myInt64, int64(2));

		CHECK(myInt64 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt8")
{
	{
		INFO("Write");
		uint8 myUInt8 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myUInt8);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		uint8 myUInt8 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myUInt8, uint8(1));

		CHECK(myUInt8 == 2);
	}

	{
		INFO("Read - Default");
		uint8 myUInt8 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myUInt8, uint8(2));

		CHECK(myUInt8 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt16")
{
	{
		INFO("Write");
		uint16 myUInt16 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myUInt16);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		uint16 myUInt16 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myUInt16, uint16(1));

		CHECK(myUInt16 == 2);
	}

	{
		INFO("Read - Default");
		uint16 myUInt16 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myUInt16, uint16(2));

		CHECK(myUInt16 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt32")
{
	{
		INFO("Write");
		uint32 myUInt32 = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myUInt32);

		str::String string = visitor;
		CHECK(string == "MyKey = 2");
	}

	{
		INFO("Read");
		uint32 myUInt32 = 1;
		eng::Visitor visitor = str::StringView("MyKey = 2");
		visitor.Read(strMyKey, myUInt32, 1u);

		CHECK(myUInt32 == 2);
	}

	{
		INFO("Read - Default");
		uint32 myUInt32 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myUInt32, 2u);

		CHECK(myUInt32 == 2);
	}
}

TEST_CASE("eng::Visitor::Enum Int8")
{
	{
		INFO("Write");
		EInt8 myEInt8 = EInt8::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEInt8);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EInt8 myEInt8 = EInt8::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEInt8, EInt8::Min);

		CHECK(myEInt8 == EInt8::Max);
	}

	{
		INFO("Read - Default");
		EInt8 myEInt8 = EInt8::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEInt8, EInt8::Max);

		CHECK(myEInt8 == EInt8::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int16")
{
	{
		INFO("Write");
		EInt16 myEInt16 = EInt16::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEInt16);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EInt16 myEInt16 = EInt16::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEInt16, EInt16::Min);

		CHECK(myEInt16 == EInt16::Max);
	}

	{
		INFO("Read - Default");
		EInt16 myEInt16 = EInt16::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEInt16, EInt16::Max);

		CHECK(myEInt16 == EInt16::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int32")
{
	{
		INFO("Write");
		EInt32 myEInt32 = EInt32::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEInt32);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EInt32 myEInt32 = EInt32::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEInt32, EInt32::Min);

		CHECK(myEInt32 == EInt32::Max);
	}

	{
		INFO("Read - Default");
		EInt32 myEInt32 = EInt32::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEInt32, EInt32::Max);

		CHECK(myEInt32 == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int64")
{
	{
		INFO("Write");
		EInt64 myEInt64 = EInt64::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEInt64);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EInt64 myEInt64 = EInt64::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEInt64, EInt64::Min);

		CHECK(myEInt64 == EInt64::Max);
	}

	{
		INFO("Read - Default");
		EInt64 myEInt64 = EInt64::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEInt64, EInt64::Max);

		CHECK(myEInt64 == EInt64::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt8")
{
	{
		INFO("Write");
		EUInt8 myEUInt8 = EUInt8::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEUInt8);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EUInt8 myEUInt8 = EUInt8::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEUInt8, EUInt8::Min);

		CHECK(myEUInt8 == EUInt8::Max);
	}

	{
		INFO("Read - Default");
		EUInt8 myEUInt8 = EUInt8::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEUInt8, EUInt8::Max);

		CHECK(myEUInt8 == EUInt8::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt16")
{
	{
		INFO("Write");
		EUInt16 myEUInt16 = EUInt16::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEUInt16);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EUInt16 myEUInt16 = EUInt16::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEUInt16, EUInt16::Min);

		CHECK(myEUInt16 == EUInt16::Max);
	}

	{
		INFO("Read - Default");
		EUInt16 myEUInt16 = EUInt16::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEUInt16, EUInt16::Max);

		CHECK(myEUInt16 == EUInt16::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt32")
{
	{
		INFO("Write");
		EUInt32 myEUInt32 = EUInt32::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEUInt32);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EUInt32 myEUInt32 = EUInt32::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEUInt32, EUInt32::Min);

		CHECK(myEUInt32 == EUInt32::Max);
	}

	{
		INFO("Read - Default");
		EUInt32 myEUInt32 = EUInt32::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEUInt32, EUInt32::Max);

		CHECK(myEUInt32 == EUInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt64")
{
	{
		INFO("Write");
		EUInt64 myEUInt64 = EUInt64::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myEUInt64);

		str::String string = visitor;
		CHECK(string == "MyKey = 'Max'");
	}

	{
		INFO("Read");
		EUInt64 myEUInt64 = EUInt64::Min;
		eng::Visitor visitor = str::StringView("MyKey = 'Max'");
		visitor.Read(strMyKey, myEUInt64, EUInt64::Min);

		CHECK(myEUInt64 == EUInt64::Max);
	}

	{
		INFO("Read - Default");
		EUInt64 myEUInt64 = EUInt64::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myEUInt64, EUInt64::Max);

		CHECK(myEUInt64 == EUInt64::Max);
	}
}

TEST_CASE("eng::Visitor::Guid")
{
	{
		INFO("Write");
		str::Guid myGuid = strGuidA;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myGuid);

		str::String string = visitor;
		CHECK(string == "MyKey = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'");
	}

	{
		INFO("Read");
		str::Guid myGuid = strGuidB;
		eng::Visitor visitor = str::StringView("MyKey = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'");
		visitor.Read(strMyKey, myGuid, strGuidB);

		CHECK(myGuid == strGuidA);
	}

	{
		INFO("Read - Default");
		str::Guid myGuid = strGuidB;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myGuid, strGuidA);

		CHECK(myGuid == strGuidA);
	}
}

TEST_CASE("eng::Visitor::Path")
{
	{
		INFO("Write");
		str::Path myPath = str::Path("PathA");
		eng::Visitor visitor;
		visitor.Write(strMyKey, myPath);

		str::String string = visitor;
		CHECK(string == "MyKey = 'PathA'");
	}

	{
		INFO("Read");
		str::Path myPath = str::Path("PathB");
		eng::Visitor visitor = str::StringView("MyKey = 'PathA'");
		visitor.Read(strMyKey, myPath, str::Path("PathB"));

		CHECK(myPath == str::Path("PathA"));
	}

	{
		INFO("Read - Default");
		str::Path myPath = str::Path("PathB");
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myPath, str::Path("PathA"));

		CHECK(myPath == str::Path("PathA"));
	}
}

TEST_CASE("eng::Visitor::Name")
{
	{
		INFO("Write");
		str::Name myName = NAME("NameA");
		eng::Visitor visitor;
		visitor.Write(strMyKey, myName);

		str::String string = visitor;
		CHECK(string == "MyKey = 'NameA'");
	}

	{
		INFO("Read");
		str::Name myName = NAME("NameB");
		eng::Visitor visitor = str::StringView("MyKey = 'NameA'");
		visitor.Read(strMyKey, myName, NAME("NameB"));

		CHECK(myName == NAME("NameA"));
	}

	{
		INFO("Read - Default");
		str::Name myName = NAME("NameB");
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myName, NAME("NameA"));

		CHECK(myName == NAME("NameA"));
	}
}

TEST_CASE("eng::Visitor::String")
{
	{
		INFO("Write");
		str::String myString = "StringA";
		eng::Visitor visitor;
		visitor.Write(strMyKey, myString);

		str::String string = visitor;
		CHECK(string == "MyKey = 'StringA'");
	}

	{
		INFO("Read");
		str::String myString = "StringB";
		eng::Visitor visitor = str::StringView("MyKey = 'StringA'");
		visitor.Read(strMyKey, myString, str::String("StringB"));

		CHECK(myString == "StringA");
	}

	{
		INFO("Read - Default");
		str::String myString = "StringB";
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myString, str::String("StringA"));

		CHECK(myString == "StringA");
	}
}

TEST_CASE("eng::Visitor::Struct")
{
	{
		INFO("Write");
		Struct myStruct = { true, 2.f, 2 };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myStruct);

		str::String string = visitor;
		CHECK(string == "[MyKey]\nBool = true\nFloat = 2.0\nInt32 = 2");
	}

	{
		INFO("Read");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("[MyKey]\n Bool = true\n Float = 2.0\n Int32 = 2");
		visitor.Read(strMyKey, myStruct, {});

		CHECK(myStruct.m_Bool == true);
		CHECK(myStruct.m_Float == 2.f);
		CHECK(myStruct.m_Int32 == 2);
	}

	{
		INFO("Read - Inline");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("MyKey = { Bool = true, Float = 2.0, Int32 = 2 }");
		visitor.Read(strMyKey, myStruct, {});

		CHECK(myStruct.m_Bool == true);
		CHECK(myStruct.m_Float == 2.f);
		CHECK(myStruct.m_Int32 == 2);
	}

	{
		INFO("Read - Default");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myStruct, { true, 2.f, 2 });

		CHECK(myStruct.m_Bool == true);
		CHECK(myStruct.m_Float == 2.f);
		CHECK(myStruct.m_Int32 == 2);
	}
}

TEST_CASE("eng::Visitor::Quaternion")
{
	{
		INFO("Write");
		Quaternion myQuaternion = Quaternion(1.f, 2.f, 3.f, 4.f);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myQuaternion);

		str::String string = visitor;
		CHECK(string == "MyKey = { W = 4.0, X = 1.0, Y = 2.0, Z = 3.0 }");
	}

	{
		INFO("Read");
		Quaternion myQuaternion = Quaternion::Identity;
		eng::Visitor visitor = str::StringView("MyKey = { W = 4.0, X = 1.0, Y = 2.0, Z = 3.0 }");
		visitor.Read(strMyKey, myQuaternion, Quaternion::Identity);

		CHECK(myQuaternion == Quaternion(1.f, 2.f, 3.f, 4.f));
	}

	{
		INFO("Read - Default");
		Quaternion myQuaternion = Quaternion::Identity;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myQuaternion, Quaternion(1.f, 2.f, 3.f, 4.f));

		CHECK(myQuaternion == Quaternion(1.f, 2.f, 3.f, 4.f));
	}
}

TEST_CASE("eng::Visitor::Rotator")
{
	{
		INFO("Write");
		Rotator myRotator = Rotator(1.f, 2.f, 3.f);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myRotator);

		str::String string = visitor;
		CHECK(string == "MyKey = { Pitch = 1.0, Roll = 3.0, Yaw = 2.0 }");
	}

	{
		INFO("Read");
		Rotator myRotator = Rotator::Zero;
		eng::Visitor visitor = str::StringView("[MyKey]\nPitch = 1.0\nYaw = 2.0\nRoll = 3.0");
		visitor.Read(strMyKey, myRotator, Rotator(1.f));

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}

	{
		INFO("Read - Inline");
		Rotator myRotator = Rotator::Zero;
		eng::Visitor visitor = str::StringView("MyKey = { Pitch = 1.0, Yaw = 2.0, Roll = 3.0 }");
		visitor.Read(strMyKey, myRotator, Rotator::Zero);

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}

	{
		INFO("Read - Default");
		Rotator myRotator = Rotator(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myRotator, Rotator(1.f, 2.f, 3.f));

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}
}


TEST_CASE("eng::Visitor::Vector2f")
{
	{
		INFO("Write");
		Vector2f myVector2f = Vector2f(2.f);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector2f);

		str::String string = visitor;
		CHECK(string == "MyKey = { X = 2.0, Y = 2.0 }");
	}

	{
		INFO("Read");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("[MyKey]\n X = 2.0\n Y = 2.0");
		visitor.Read(strMyKey, myVector2f, Vector2f(1.f));

		CHECK(myVector2f == Vector2f(2.f));
	}

	{
		INFO("Read - Inline");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("MyKey = { X = 2.0, Y = 2.0 }");
		visitor.Read(strMyKey, myVector2f, Vector2f(1.f));

		CHECK(myVector2f == Vector2f(2.f));
	}

	{
		INFO("Read - Default");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector2f, Vector2f(2.f));

		CHECK(myVector2f == Vector2f(2.f));
	}
}

TEST_CASE("eng::Visitor::Vector2i")
{
	{
		INFO("Write");
		Vector2i myVector2i = Vector2i(2);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector2i);

		str::String string = visitor;
		CHECK(string == "MyKey = { X = 2, Y = 2 }");
	}

	{
		INFO("Read");
		Vector2i myVector2i = Vector2i(1);
		eng::Visitor visitor = str::StringView("MyKey = { X = 2, Y = 2 }");
		visitor.Read(strMyKey, myVector2i, Vector2i(1));

		CHECK(myVector2i == Vector2i(2));
	}

	{
		INFO("Read - Default");
		Vector2i myVector2i = Vector2i(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector2i, Vector2i(2));

		CHECK(myVector2i == Vector2i(2));
	}
}

TEST_CASE("eng::Visitor::Vector2u")
{
	{
		INFO("Write");
		Vector2u myVector2u = Vector2u(2);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector2u);

		str::String string = visitor;
		CHECK(string == "MyKey = { X = 2, Y = 2 }");
	}

	{
		INFO("Read");
		Vector2u myVector2u = Vector2u(1);
		eng::Visitor visitor = str::StringView("MyKey = { X = 2, Y = 2 }");
		visitor.Read(strMyKey, myVector2u, Vector2u(1));

		CHECK(myVector2u == Vector2u(2));
	}

	{
		INFO("Read - Default");
		Vector2u myVector2u = Vector2u(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector2u, Vector2u(2));

		CHECK(myVector2u == Vector2u(2));
	}
}

TEST_CASE("eng::Visitor::Vector3f")
{
	{
		INFO("Write");
		Vector3f myVector3f = Vector3f(2.f);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector3f);

		str::String string = visitor;
		CHECK(string == "MyKey = { X = 2.0, Y = 2.0, Z = 2.0 }");
	}

	{
		INFO("Read");
		Vector3f myVector3f = Vector3f(1.f);
		eng::Visitor visitor = str::StringView("MyKey = { X = 2.0, Y = 2.0, Z = 2.0 }");
		visitor.Read(strMyKey, myVector3f, Vector3f(1.f));

		CHECK(myVector3f == Vector3f(2.f));
	}

	{
		INFO("Read - Default");
		Vector3f myVector3f = Vector3f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector3f, Vector3f(2.f));

		CHECK(myVector3f == Vector3f(2.f));
	}
}

TEST_CASE("eng::Visitor::Vector3i")
{
	{
		INFO("Write");
		Vector3i myVector3i = Vector3i(2);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector3i);

		str::String string = visitor;
		CHECK(string == "MyKey = { X = 2, Y = 2, Z = 2 }");
	}

	{
		INFO("Read");
		Vector3i myVector3i = Vector3i(1);
		eng::Visitor visitor = str::StringView("MyKey = { X = 2, Y = 2, Z = 2 }");
		visitor.Read(strMyKey, myVector3i, Vector3i(1));

		CHECK(myVector3i == Vector3i(2));
	}

	{
		INFO("Read - Default");
		Vector3i myVector3i = Vector3i(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector3i, Vector3i(2));

		CHECK(myVector3i == Vector3i(2));
	}
}

TEST_CASE("eng::Visitor::Vector4f")
{
	{
		INFO("Write");
		Vector4f myVector4f = Vector4f(2.f);
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVector4f);

		str::String string = visitor;
		CHECK(string == "MyKey = { W = 2.0, X = 2.0, Y = 2.0, Z = 2.0 }");
	}

	{
		INFO("Read");
		Vector4f myVector4f = Vector4f(1.f);
		eng::Visitor visitor = str::StringView("MyKey = { W = 2.0, X = 2.0, Y = 2.0, Z = 2.0 }");
		visitor.Read(strMyKey, myVector4f, Vector4f(1.f));

		CHECK(myVector4f == Vector4f(2.f));
	}

	{
		INFO("Read - Default");
		Vector4f myVector4f = Vector4f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVector4f, Vector4f(2.f));

		CHECK(myVector4f == Vector4f(2.f));
	}
}

TEST_CASE("eng::Visitor::Array<Enum>")
{
	{
		INFO("Write");
		Array<EInt32> myArray = { EInt32::Max };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ 'Max' ]");
	}

	{
		INFO("Read");
		Array<EInt32> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ 'Max' ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Array<EInt32> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { EInt32::Max });

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Array<Int32>")
{
	{
		INFO("Write");
		Array<int32> myArray = { 2 };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ 2 ]");
	}

	{
		INFO("Read");
		Array<int32> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ 2 ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == 2);
	}

	{
		INFO("Read - Default");
		Array<int32> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { 2 });

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == 2);
	}
}

TEST_CASE("eng::Visitor::Array<Variant>")
{
	{
		INFO("Write");
		Array<Variant<bool, int32>> myArray = { 3 };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "[[MyKey]]\nint = 3");
	}

	{
		INFO("Read");
		Array<Variant<bool, int32>> myArray;
		eng::Visitor visitor = str::StringView("[[MyKey]]\nint = 3");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Variant<bool, int32>{ 3 });
	}
}

TEST_CASE("eng::Visitor::Array<Vector2f>")
{
	{
		INFO("Write");
		Array<Vector2f> myArray = { Vector2f::One };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ { X = 1.0, Y = 1.0 } ]");
	}

	{
		INFO("Read");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("[[MyKey]]\n X = 1.0\n Y = 1.0");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Vector2f::One);
	}

	{
		INFO("Read - Inline");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ { X = 1.0, Y = 1.0 } ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Vector2f::One);
	}

	{
		INFO("Read - Multiline");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("[[MyKey]]\n X = 1.0\n Y = 1.0\n [[MyKey]]\n X = 2.0\n Y = 2.0");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 2);
		CHECK(myArray[0] == Vector2f(1.f));
		CHECK(myArray[1] == Vector2f(2.f));
	}

	{
		INFO("Read - Default");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { Vector2f::One });

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Vector2f::One);
	}
}

TEST_CASE("eng::Visitor::Array<Array<Enum>>")
{
	{
		INFO("Write");
		Array<Array<EInt32>> myArray = { { EInt32::Max } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ [ 'Max' ] ]");
	}

	{
		INFO("Read");
		Array<Array<EInt32>> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ [ 'Max' ] ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Array<Array<EInt32>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { { EInt32::Max } });

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Array<Array<Int32>>")
{
	{
		INFO("Write");
		Array<Array<int32>> myArray = { { 2 } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ [ 2 ] ]");
	}

	{
		INFO("Read - Inline");
		Array<Array<int32>> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ [ 2 ] ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == 2);
	}

	{
		INFO("Read - Default");
		Array<Array<int32>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { { 2 } });

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == 2);
	}
}

TEST_CASE("eng::Visitor::Array<Array<Vector2f>>")
{
	{
		INFO("Write");
		Array<Array<Vector2f>> myArray = { { Vector2f::One } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myArray);

		str::String string = visitor;
		CHECK(string == "MyKey = [ [ { X = 1.0, Y = 1.0 } ] ]");
	}

	{
		INFO("Read");
		Array<Array<Vector2f>> myArray;
		eng::Visitor visitor = str::StringView("MyKey = [ [ { X = 1.0, Y = 1.0 } ] ]");
		visitor.Read(strMyKey, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == Vector2f::One);
	}

	{
		INFO("Read - Default");
		Array<Array<Vector2f>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myArray, { { Vector2f::One } });

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == Vector2f::One);
	}
}

TEST_CASE("eng::Visitor::Map<Enum>")
{
	{
		INFO("Write");
		Map<str::String, EInt32> myMap = { { "A", EInt32::Max } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey]\nA = 'Max'");
	}

	{
		INFO("Read");
		Map<str::String, EInt32> myMap;
		eng::Visitor visitor = str::StringView("[MyKey]\nA = 'Max'");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Map<str::String, EInt32> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myMap, { { "A", EInt32::Max } });

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Map<Int32>")
{
	{
		INFO("Write");
		Map<str::String, int32> myMap = { { "A", 2 } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey]\nA = 2");
	}

	{
		INFO("Read");
		Map<str::String, int32> myMap;
		eng::Visitor visitor = str::StringView("[MyKey]\nA = 2");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == 2);
	}

	{
		INFO("Read - Default");
		Map<str::String, int32> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myMap, { { "A", 2 } });

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == 2);
	}
}

TEST_CASE("eng::Visitor::Map<Vector2f>")
{
	{
		INFO("Write");
		Map<str::String, Vector2f> myMap = { { "A", Vector2f::One } };
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey]\nA = { X = 1.0, Y = 1.0 }");
	}

	{
		INFO("Read");
		Map<str::String, Vector2f> myMap;
		eng::Visitor visitor = str::StringView("[MyKey]\nA = { X = 1.0, Y = 1.0 }");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == Vector2f::One);
	}

	{
		INFO("Read - Default");
		Map<str::String, Vector2f> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myMap, { { "A", Vector2f::One } });

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == Vector2f::One);
	}
}

TEST_CASE("eng::Visitor::Map<Map<Enum>>")
{
	{
		INFO("Write");
		Map<str::String, Map<str::String, EInt32>> myMap;
		myMap["A"]["B"] = EInt32::Max;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey.A]\nB = 'Max'");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, EInt32>> myMap;
		eng::Visitor visitor = str::StringView("[MyKey.A]\nB = 'Max'");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Map<str::String, Map<str::String, EInt32>> myMap;
		eng::Visitor visitor = str::StringView("");

		{
			Map<str::String, Map<str::String, EInt32>> myDefault;
			myDefault["A"]["B"] = EInt32::Max;
			visitor.Read(strMyKey, myMap, myDefault);
		}

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Map<Map<Int32>>")
{
	{
		INFO("Write");
		Map<str::String, Map<str::String, int32>> myMap;
		myMap["A"]["B"] = 2;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey.A]\nB = 2");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, int32>> myMap;
		eng::Visitor visitor = str::StringView("[MyKey.A]\nB = 2");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == 2);
	}

	{
		INFO("Read - Default");
		Map<str::String, Map<str::String, int32>> myMap;
		eng::Visitor visitor = str::StringView("");

		{
			Map<str::String, Map<str::String, int32>> myDefault;
			myDefault["A"]["B"] = 2;
			visitor.Read(strMyKey, myMap, myDefault);
		}

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == 2);
	}
}

TEST_CASE("eng::Visitor::Map<Map<Vector2f>>")
{
	{
		INFO("Write");
		Map<str::String, Map<str::String, Vector2f>> myMap;
		myMap["A"]["B"] = Vector2f::One;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myMap);

		str::String string = visitor;
		CHECK(string == "[MyKey.A]\nB = { X = 1.0, Y = 1.0 }");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, Vector2f>> myMap;
		eng::Visitor visitor = str::StringView("[MyKey.A]\nB = { X = 1.0, Y = 1.0 }");
		visitor.Read(strMyKey, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == Vector2f::One);
	}

	{
		INFO("Read - Default");
		Map<str::String, Map<str::String, Vector2f>> myMap;
		eng::Visitor visitor = str::StringView("");

		{
			Map<str::String, Map<str::String, Vector2f>> myDefault;
			myDefault["A"]["B"] = Vector2f::One;
			visitor.Read(strMyKey, myMap, myDefault);
		}

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == Vector2f::One);
	}
}

TEST_CASE("eng::Visitor::Optional<int32>")
{
	{
		INFO("Write. With Value.");
		Optional<int32> myOptional = 3;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myOptional);

		str::String string = visitor;
		CHECK(string == "MyKey = 3");
	}

	{
		INFO("Write. Without Value.");
		Optional<int32> myOptional = std::nullopt;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myOptional);

		str::String string = visitor;
		CHECK(string.empty());
	}

	{
		INFO("Read");
		Optional<int32> myOptional;
		eng::Visitor visitor = str::StringView("MyKey = 3");
		visitor.Read(strMyKey, myOptional, { 0 });

		REQUIRE(myOptional);
		CHECK(*myOptional == 3);
	}

	{
		INFO("Read - Default");
		Optional<int32> myOptional;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myOptional, { 3 });

		REQUIRE(myOptional);
		CHECK(*myOptional == 3);
	}
}

TEST_CASE("eng::Visitor::Optional<Optional<int32>>")
{
	{
		INFO("Write. With Value.");
		Optional<Optional<int32>> myOptional = 3;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myOptional);

		str::String string = visitor;
		CHECK(string == "MyKey = 3");
	}

	{
		INFO("Write. With outer value.");
		Optional<int32> myInner = std::nullopt;
		Optional<Optional<int32>> myOuter = myInner;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myOuter);

		str::String string = visitor;
		CHECK(string.empty());
	}

	{
		INFO("Write. Without Value.");
		Optional<Optional<int32>> myOptional = std::nullopt;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myOptional);

		str::String string = visitor;
		CHECK(string.empty());
	}

	{
		INFO("Read");
		Optional<Optional<int32>> myOptional;
		eng::Visitor visitor = str::StringView("MyKey = 3");
		visitor.Read(strMyKey, myOptional, { 0 });

		REQUIRE(myOptional);
		CHECK(*myOptional == 3);
	}

	{
		INFO("Read - Default");
		Optional<Optional<int32>> myOptional;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myOptional, { 3 });

		REQUIRE(myOptional);
		CHECK(*myOptional == 3);
	}
}

TEST_CASE("eng::Visitor::Variant<bool, int32>")
{
	{
		INFO("Write");
		Variant<bool, int32> myVariant = 3;
		eng::Visitor visitor;
		visitor.Write(strMyKey, myVariant);

		str::String string = visitor;
		CHECK(string == "[MyKey]\nint = 3");
	}

	{
		INFO("Read");
		Variant<bool, int32> myVariant;
		eng::Visitor visitor = str::StringView("MyKey = { int = 3 }");
		visitor.Read(strMyKey, myVariant, Variant<bool, int32>{0});

		REQUIRE(std::holds_alternative<int32>(myVariant));
		CHECK(std::get<int32>(myVariant) == 3);
	}

	{
		INFO("Read - Default");
		Variant<bool, int32> myVariant;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMyKey, myVariant, Variant<bool, int32>{3});

		REQUIRE(std::holds_alternative<int32>(myVariant));
		CHECK(std::get<int32>(myVariant) == 3);
	}
}

TEST_CASE("eng::Visitor::Iterator Range-based for loop")
{
	eng::Visitor visitor = str::StringView("First = {Bool = true}\nSecond = {Number = 1}");

	int32 index = 0;
	Array<str::StringView> results = { "First", "Bool", "Second", "Number" };
	for (const str::StringView& keyA : visitor)
	{
		CHECK(keyA == results[index++]);
		for (const str::StringView& keyB : visitor)
			CHECK(keyB == results[index++]);
	}
}