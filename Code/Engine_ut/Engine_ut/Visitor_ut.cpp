#include <Catch2/catch.hpp>

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
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
	const str::Guid& strGuidA = GUID("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
	const str::Guid& strGuidB = GUID("bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");

	constexpr str::StringView strBool = "Bool";
	constexpr str::StringView strFloat = "Float";
	constexpr str::StringView strDouble = "Double";
	constexpr str::StringView strInt8 = "Int8";
	constexpr str::StringView strInt16 = "Int16";
	constexpr str::StringView strInt32 = "Int32";
	constexpr str::StringView strInt64 = "Int64";
	constexpr str::StringView strUInt8 = "UInt8";
	constexpr str::StringView strUInt16 = "UInt16";
	constexpr str::StringView strUInt32 = "UInt32";

	constexpr str::StringView strEnumInt8 = "EnumInt8";
	constexpr str::StringView strEnumInt16 = "EnumInt16";
	constexpr str::StringView strEnumInt32 = "EnumInt32";
	constexpr str::StringView strEnumInt64 = "EnumInt64";
	constexpr str::StringView strEnumUInt8 = "EnumUInt8";
	constexpr str::StringView strEnumUInt16 = "EnumUInt16";
	constexpr str::StringView strEnumUInt32 = "EnumUInt32";

	constexpr str::StringView strGuid = "Guid";
	constexpr str::StringView strName = "Name";
	constexpr str::StringView strPath = "Path";
	constexpr str::StringView strString = "String";

	constexpr str::StringView strStruct = "Struct";

	constexpr str::StringView strQuaternion = "Quaternion";
	constexpr str::StringView strRotator = "Rotator";
	constexpr str::StringView strVector2f = "Vector2f";
	constexpr str::StringView strVector2i = "Vector2i";
	constexpr str::StringView strVector2u = "Vector2u";
	constexpr str::StringView strVector3f = "Vector3f";
	constexpr str::StringView strVector3i = "Vector3i";
	constexpr str::StringView strVector4f = "Vector4f";

	constexpr str::StringView strArrayEnum = "Array<Enum>";
	constexpr str::StringView strArrayInt32 = "Array<Int32>";
	constexpr str::StringView strArrayVector2f = "Array<Vector2f>";
	constexpr str::StringView strArrayArrayEnum = "Array<Array<Enum>>";
	constexpr str::StringView strArrayArrayInt32 = "Array<Array<Int32>>";
	constexpr str::StringView strArrayArrayVector2f = "Array<Array<Vector2f>>";

	constexpr str::StringView strMapEnum = "Map<Enum>";
	constexpr str::StringView strMapInt32 = "Map<Int32>";
	constexpr str::StringView strMapVector2f = "Map<Vector2f>";
	constexpr str::StringView strMapMapEnum = "Map<Map<Enum>>";
	constexpr str::StringView strMapMapInt32 = "Map<Map<Int32>>";
	constexpr str::StringView strMapMapVector2f = "Map<Map<Vector2f>>";

	constexpr str::StringView strVariant = "Variant";

	enum class EInt8 : int8 { Min = -INT8_MAX, Max = +INT8_MAX };
	enum class EInt16 : int16 { Min = -INT16_MAX, Max = +INT16_MAX };
	enum class EInt32 : int32 { Min = -INT32_MAX, Max = +INT32_MAX };
	enum class EInt64 : int64 { Min = -INT64_MAX, Max = +INT64_MAX };

	enum class EUInt8 : uint8 { Min = 0, Max = UINT8_MAX };
	enum class EUInt16 : uint16 { Min = 0, Max = UINT16_MAX };
	enum class EUInt32 : uint32 { Min = 0, Max = UINT32_MAX };

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
		visitor.Write(strBool, myBool);

		str::String string = visitor;
		CHECK(string == "Bool = true");
	}

	{
		INFO("Read");
		bool myBool = false;
		eng::Visitor visitor = str::StringView("Bool = true");
		visitor.Read(strBool, myBool, false);

		CHECK(myBool == true);
	}

	{
		INFO("Read - Default");
		bool myBool = false;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strBool, myBool, true);

		CHECK(myBool == true);
	}
}

TEST_CASE("eng::Visitor::Float")
{
	{
		INFO("Write");
		float myFloat = 2.f;
		eng::Visitor visitor;
		visitor.Write(strFloat, myFloat);

		str::String string = visitor;
		CHECK(string == "Float = 2.0");
	}

	{
		INFO("Read");
		float myFloat = 1.f;
		eng::Visitor visitor = str::StringView("Float = 2.0");
		visitor.Read(strFloat, myFloat, 1.f);

		CHECK(myFloat == 2.f);
	}

	{
		INFO("Read - Default");
		float myFloat = 1.f;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strFloat, myFloat, 2.f);

		CHECK(myFloat == 2.f);
	}
}

TEST_CASE("eng::Visitor::Double")
{
	{
		INFO("Write");
		double myDouble = 2.0;
		eng::Visitor visitor;
		visitor.Write(strDouble, myDouble);

		str::String string = visitor;
		CHECK(string == "Double = 2.0");
	}

	{
		INFO("Read");
		double myDouble = 1.0;
		eng::Visitor visitor = str::StringView("Double = 2.0");
		visitor.Read(strDouble, myDouble, 1.0);

		CHECK(myDouble == 2.0);
	}

	{
		INFO("Read - Default");
		double myDouble = 1.0;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strDouble, myDouble, 2.0);

		CHECK(myDouble == 2.0);
	}
}

TEST_CASE("eng::Visitor::Int8")
{
	{
		INFO("Write");
		int8 myInt8 = 2;
		eng::Visitor visitor;
		visitor.Write(strInt8, myInt8);

		str::String string = visitor;
		CHECK(string == "Int8 = 2");
	}

	{
		INFO("Read");
		int8 myInt8 = 1;
		eng::Visitor visitor = str::StringView("Int8 = 2");
		visitor.Read(strInt8, myInt8, int8(1));

		CHECK(myInt8 == 2);
	}

	{
		INFO("Read - Default");
		int8 myInt8 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strInt8, myInt8, int8(2));

		CHECK(myInt8 == 2);
	}
}

TEST_CASE("eng::Visitor::Int16")
{
	{
		INFO("Write");
		int16 myInt16 = 2;
		eng::Visitor visitor;
		visitor.Write(strInt16, myInt16);

		str::String string = visitor;
		CHECK(string == "Int16 = 2");
	}

	{
		INFO("Read");
		int16 myInt16 = 1;
		eng::Visitor visitor = str::StringView("Int16 = 2");
		visitor.Read(strInt16, myInt16, int16(1));

		CHECK(myInt16 == 2);
	}

	{
		INFO("Read - Default");
		int16 myInt16 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strInt16, myInt16, int16(2));

		CHECK(myInt16 == 2);
	}
}

TEST_CASE("eng::Visitor::Int32")
{
	{
		INFO("Write");
		int32 myInt32 = 2;
		eng::Visitor visitor;
		visitor.Write(strInt32, myInt32);

		str::String string = visitor;
		CHECK(string == "Int32 = 2");
	}

	{
		INFO("Read");
		int32 myInt32 = 1;
		eng::Visitor visitor = str::StringView("Int32 = 2");
		visitor.Read(strInt32, myInt32, 1);

		CHECK(myInt32 == 2);
	}

	{
		INFO("Read - Default");
		int32 myInt32 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strInt32, myInt32, 2);

		CHECK(myInt32 == 2);
	}
}

TEST_CASE("eng::Visitor::Int64")
{
	{
		INFO("Write");
		int64 myInt64 = 2;
		eng::Visitor visitor;
		visitor.Write(strInt64, myInt64);

		str::String string = visitor;
		CHECK(string == "Int64 = 2");
	}

	{
		INFO("Read");
		int64 myInt64 = 1;
		eng::Visitor visitor = str::StringView("Int64 = 2");
		visitor.Read(strInt64, myInt64, int64(1));

		CHECK(myInt64 == 2);
	}

	{
		INFO("Read - Default");
		int64 myInt64 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strInt64, myInt64, int64(2));

		CHECK(myInt64 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt8")
{
	{
		INFO("Write");
		uint8 myUInt8 = 2;
		eng::Visitor visitor;
		visitor.Write(strUInt8, myUInt8);

		str::String string = visitor;
		CHECK(string == "UInt8 = 2");
	}

	{
		INFO("Read");
		uint8 myUInt8 = 1;
		eng::Visitor visitor = str::StringView("UInt8 = 2");
		visitor.Read(strUInt8, myUInt8, uint8(1));

		CHECK(myUInt8 == 2);
	}

	{
		INFO("Read - Default");
		uint8 myUInt8 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strUInt8, myUInt8, uint8(2));

		CHECK(myUInt8 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt16")
{
	{
		INFO("Write");
		uint16 myUInt16 = 2;
		eng::Visitor visitor;
		visitor.Write(strUInt16, myUInt16);

		str::String string = visitor;
		CHECK(string == "UInt16 = 2");
	}

	{
		INFO("Read");
		uint16 myUInt16 = 1;
		eng::Visitor visitor = str::StringView("UInt16 = 2");
		visitor.Read(strUInt16, myUInt16, uint16(1));

		CHECK(myUInt16 == 2);
	}

	{
		INFO("Read - Default");
		uint16 myUInt16 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strUInt16, myUInt16, uint16(2));

		CHECK(myUInt16 == 2);
	}
}

TEST_CASE("eng::Visitor::UInt32")
{
	{
		INFO("Write");
		uint32 myUInt32 = 2;
		eng::Visitor visitor;
		visitor.Write(strUInt32, myUInt32);

		str::String string = visitor;
		CHECK(string == "UInt32 = 2");
	}

	{
		INFO("Read");
		uint32 myUInt32 = 1;
		eng::Visitor visitor = str::StringView("UInt32 = 2");
		visitor.Read(strUInt32, myUInt32, 1u);

		CHECK(myUInt32 == 2);
	}

	{
		INFO("Read - Default");
		uint32 myUInt32 = 1;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strUInt32, myUInt32, 2u);

		CHECK(myUInt32 == 2);
	}
}

TEST_CASE("eng::Visitor::Enum Int8")
{
	{
		INFO("Write");
		EInt8 myEInt8 = EInt8::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt8, myEInt8);

		str::String string = visitor;
		CHECK(string == "EnumInt8 = 127");
	}

	{
		INFO("Read");
		EInt8 myEInt8 = EInt8::Min;
		eng::Visitor visitor = str::StringView("EnumInt8 = 127");
		visitor.Read(strEnumInt8, myEInt8, EInt8::Min);

		CHECK(myEInt8 == EInt8::Max);
	}

	{
		INFO("Read - Default");
		EInt8 myEInt8 = EInt8::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt8, myEInt8, EInt8::Max);

		CHECK(myEInt8 == EInt8::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int16")
{
	{
		INFO("Write");
		EInt16 myEInt16 = EInt16::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt16, myEInt16);

		str::String string = visitor;
		CHECK(string == "EnumInt16 = 32767");
	}

	{
		INFO("Read");
		EInt16 myEInt16 = EInt16::Min;
		eng::Visitor visitor = str::StringView("EnumInt16 = 32767");
		visitor.Read(strEnumInt16, myEInt16, EInt16::Min);

		CHECK(myEInt16 == EInt16::Max);
	}

	{
		INFO("Read - Default");
		EInt16 myEInt16 = EInt16::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt16, myEInt16, EInt16::Max);

		CHECK(myEInt16 == EInt16::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int32")
{
	{
		INFO("Write");
		EInt32 myEInt32 = EInt32::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt32, myEInt32);

		str::String string = visitor;
		CHECK(string == "EnumInt32 = 2147483647");
	}

	{
		INFO("Read");
		EInt32 myEInt32 = EInt32::Min;
		eng::Visitor visitor = str::StringView("EnumInt32 = 2147483647");
		visitor.Read(strEnumInt32, myEInt32, EInt32::Min);

		CHECK(myEInt32 == EInt32::Max);
	}

	{
		INFO("Read - Default");
		EInt32 myEInt32 = EInt32::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt32, myEInt32, EInt32::Max);

		CHECK(myEInt32 == EInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Enum Int64")
{
	{
		INFO("Write");
		EInt64 myEInt64 = EInt64::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt64, myEInt64);

		str::String string = visitor;
		CHECK(string == "EnumInt64 = 9223372036854775807");
	}

	{
		INFO("Read");
		EInt64 myEInt64 = EInt64::Min;
		eng::Visitor visitor = str::StringView("EnumInt64 = 9223372036854775807");
		visitor.Read(strEnumInt64, myEInt64, EInt64::Min);

		CHECK(myEInt64 == EInt64::Max);
	}

	{
		INFO("Read - Default");
		EInt64 myEInt64 = EInt64::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt64, myEInt64, EInt64::Max);

		CHECK(myEInt64 == EInt64::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt8")
{
	{
		INFO("Write");
		EUInt8 myEUInt8 = EUInt8::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumUInt8, myEUInt8);

		str::String string = visitor;
		CHECK(string == "EnumUInt8 = 255");
	}

	{
		INFO("Read");
		EUInt8 myEUInt8 = EUInt8::Min;
		eng::Visitor visitor = str::StringView("EnumUInt8 = 255");
		visitor.Read(strEnumUInt8, myEUInt8, EUInt8::Min);

		CHECK(myEUInt8 == EUInt8::Max);
	}

	{
		INFO("Read - Default");
		EUInt8 myEUInt8 = EUInt8::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumUInt8, myEUInt8, EUInt8::Max);

		CHECK(myEUInt8 == EUInt8::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt16")
{
	{
		INFO("Write");
		EUInt16 myEUInt16 = EUInt16::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt16, myEUInt16);

		str::String string = visitor;
		CHECK(string == "EnumInt16 = 65535");
	}

	{
		INFO("Read");
		EUInt16 myEUInt16 = EUInt16::Min;
		eng::Visitor visitor = str::StringView("EnumInt16 = 65535");
		visitor.Read(strEnumInt16, myEUInt16, EUInt16::Min);

		CHECK(myEUInt16 == EUInt16::Max);
	}

	{
		INFO("Read - Default");
		EUInt16 myEUInt16 = EUInt16::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt16, myEUInt16, EUInt16::Max);

		CHECK(myEUInt16 == EUInt16::Max);
	}
}

TEST_CASE("eng::Visitor::Enum UInt32")
{
	{
		INFO("Write");
		EUInt32 myEUInt32 = EUInt32::Max;
		eng::Visitor visitor;
		visitor.Write(strEnumInt32, myEUInt32);

		str::String string = visitor;
		CHECK(string == "EnumInt32 = 4294967295");
	}

	{
		INFO("Read");
		EUInt32 myEUInt32 = EUInt32::Min;
		eng::Visitor visitor = str::StringView("EnumInt32 = 4294967295");
		visitor.Read(strEnumInt32, myEUInt32, EUInt32::Min);

		CHECK(myEUInt32 == EUInt32::Max);
	}

	{
		INFO("Read - Default");
		EUInt32 myEUInt32 = EUInt32::Min;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strEnumInt32, myEUInt32, EUInt32::Max);

		CHECK(myEUInt32 == EUInt32::Max);
	}
}

TEST_CASE("eng::Visitor::Guid")
{
	{
		INFO("Write");
		str::Guid myGuid = strGuidA;
		eng::Visitor visitor;
		visitor.Write(strGuid, myGuid);

		str::String string = visitor;
		CHECK(string == "Guid = 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa'");
	}

	{
		INFO("Read");
		str::Guid myGuid = strGuidB;
		eng::Visitor visitor = str::StringView("Guid = 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa'");
		visitor.Read(strGuid, myGuid, strGuidB);

		CHECK(myGuid == strGuidA);
	}

	{
		INFO("Read - Default");
		str::Guid myGuid = strGuidB;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strGuid, myGuid, strGuidA);

		CHECK(myGuid == strGuidA);
	}
}

TEST_CASE("eng::Visitor::Path")
{
	{
		INFO("Write");
		str::Path myPath = str::Path("PathA");
		eng::Visitor visitor;
		visitor.Write(strPath, myPath);

		str::String string = visitor;
		CHECK(string == "Path = 'PathA'");
	}

	{
		INFO("Read");
		str::Path myPath = str::Path("PathB");
		eng::Visitor visitor = str::StringView("Path = 'PathA'");
		visitor.Read(strPath, myPath, str::Path("PathB"));

		CHECK(myPath == str::Path("PathA"));
	}

	{
		INFO("Read - Default");
		str::Path myPath = str::Path("PathB");
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strPath, myPath, str::Path("PathA"));

		CHECK(myPath == str::Path("PathA"));
	}
}

TEST_CASE("eng::Visitor::Name")
{
	{
		INFO("Write");
		str::Name myName = NAME("NameA");
		eng::Visitor visitor;
		visitor.Write(strName, myName);

		str::String string = visitor;
		CHECK(string == "Name = 'NameA'");
	}

	{
		INFO("Read");
		str::Name myName = NAME("NameB");
		eng::Visitor visitor = str::StringView("Name = 'NameA'");
		visitor.Read(strName, myName, NAME("NameB"));

		CHECK(myName == NAME("NameA"));
	}

	{
		INFO("Read - Default");
		str::Name myName = NAME("NameB");
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strName, myName, NAME("NameA"));

		CHECK(myName == NAME("NameA"));
	}
}

TEST_CASE("eng::Visitor::String")
{
	{
		INFO("Write");
		str::String myString = "StringA";
		eng::Visitor visitor;
		visitor.Write(strString, myString);

		str::String string = visitor;
		CHECK(string == "String = 'StringA'");
	}

	{
		INFO("Read");
		str::String myString = "StringB";
		eng::Visitor visitor = str::StringView("String = 'StringA'");
		visitor.Read(strString, myString, str::String("StringB"));

		CHECK(myString == "StringA");
	}

	{
		INFO("Read - Default");
		str::String myString = "StringB";
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strString, myString, str::String("StringA"));

		CHECK(myString == "StringA");
	}
}

TEST_CASE("eng::Visitor::Struct")
{
	{
		INFO("Write");
		Struct myStruct = { true, 2.f, 2 };
		eng::Visitor visitor;
		visitor.Write(strStruct, myStruct);

		str::String string = visitor;
		CHECK(string == "[Struct]\nBool = true\nFloat = 2.0\nInt32 = 2");
	}

	{
		INFO("Read");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("[Struct]\n Bool = true\n Float = 2.0\n Int32 = 2");
		visitor.Read(strStruct, myStruct, {});

		CHECK(myStruct.m_Bool == true);
		CHECK(myStruct.m_Float == 2.f);
		CHECK(myStruct.m_Int32 == 2);
	}

	{
		INFO("Read - Inline");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("Struct = { Bool = true, Float = 2.0, Int32 = 2 }");
		visitor.Read(strStruct, myStruct, {});

		CHECK(myStruct.m_Bool == true);
		CHECK(myStruct.m_Float == 2.f);
		CHECK(myStruct.m_Int32 == 2);
	}

	{
		INFO("Read - Default");
		Struct myStruct;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strStruct, myStruct, { true, 2.f, 2 });

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
		visitor.Write(strQuaternion, myQuaternion);

		str::String string = visitor;
		CHECK(string == "Quaternion = { W = 4.0, X = 1.0, Y = 2.0, Z = 3.0 }");
	}

	{
		INFO("Read");
		Quaternion myQuaternion = Quaternion::Identity;
		eng::Visitor visitor = str::StringView("Quaternion = { W = 4.0, X = 1.0, Y = 2.0, Z = 3.0 }");
		visitor.Read(strQuaternion, myQuaternion, Quaternion::Identity);

		CHECK(myQuaternion == Quaternion(1.f, 2.f, 3.f, 4.f));
	}

	{
		INFO("Read - Default");
		Quaternion myQuaternion = Quaternion::Identity;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strQuaternion, myQuaternion, Quaternion(1.f, 2.f, 3.f, 4.f));

		CHECK(myQuaternion == Quaternion(1.f, 2.f, 3.f, 4.f));
	}
}

TEST_CASE("eng::Visitor::Rotator")
{
	{
		INFO("Write");
		Rotator myRotator = Rotator(1.f, 2.f, 3.f);
		eng::Visitor visitor;
		visitor.Write(strRotator, myRotator);

		str::String string = visitor;
		CHECK(string == "Rotator = { Pitch = 1.0, Roll = 3.0, Yaw = 2.0 }");
	}

	{
		INFO("Read");
		Rotator myRotator = Rotator::Zero;
		eng::Visitor visitor = str::StringView("[Rotator]\nPitch = 1.0\nYaw = 2.0\nRoll = 3.0");
		visitor.Read(strRotator, myRotator, Rotator(1.f));

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}

	{
		INFO("Read - Inline");
		Rotator myRotator = Rotator::Zero;
		eng::Visitor visitor = str::StringView("Rotator = { Pitch = 1.0, Yaw = 2.0, Roll = 3.0 }");
		visitor.Read(strRotator, myRotator, Rotator::Zero);

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}

	{
		INFO("Read - Default");
		Rotator myRotator = Rotator(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strRotator, myRotator, Rotator(1.f, 2.f, 3.f));

		CHECK(myRotator == Rotator(1.f, 2.f, 3.f));
	}
}


TEST_CASE("eng::Visitor::Vector2f")
{
	{
		INFO("Write");
		Vector2f myVector2f = Vector2f(2.f);
		eng::Visitor visitor;
		visitor.Write(strVector2f, myVector2f);

		str::String string = visitor;
		CHECK(string == "Vector2f = { X = 2.0, Y = 2.0 }");
	}

	{
		INFO("Read");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("[Vector2f]\n X = 2.0\n Y = 2.0");
		visitor.Read(strVector2f, myVector2f, Vector2f(1.f));

		CHECK(myVector2f == Vector2f(2.f));
	}

	{
		INFO("Read - Inline");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("Vector2f = { X = 2.0, Y = 2.0 }");
		visitor.Read(strVector2f, myVector2f, Vector2f(1.f));

		CHECK(myVector2f == Vector2f(2.f));
	}

	{
		INFO("Read - Default");
		Vector2f myVector2f = Vector2f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector2f, myVector2f, Vector2f(2.f));

		CHECK(myVector2f == Vector2f(2.f));
	}
}

TEST_CASE("eng::Visitor::Vector2i")
{
	{
		INFO("Write");
		Vector2i myVector2i = Vector2i(2);
		eng::Visitor visitor;
		visitor.Write(strVector2i, myVector2i);

		str::String string = visitor;
		CHECK(string == "Vector2i = { X = 2, Y = 2 }");
	}

	{
		INFO("Read");
		Vector2i myVector2i = Vector2i(1);
		eng::Visitor visitor = str::StringView("Vector2i = { X = 2, Y = 2 }");
		visitor.Read(strVector2i, myVector2i, Vector2i(1));

		CHECK(myVector2i == Vector2i(2));
	}

	{
		INFO("Read - Default");
		Vector2i myVector2i = Vector2i(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector2i, myVector2i, Vector2i(2));

		CHECK(myVector2i == Vector2i(2));
	}
}

TEST_CASE("eng::Visitor::Vector2u")
{
	{
		INFO("Write");
		Vector2u myVector2u = Vector2u(2);
		eng::Visitor visitor;
		visitor.Write(strVector2u, myVector2u);

		str::String string = visitor;
		CHECK(string == "Vector2u = { X = 2, Y = 2 }");
	}

	{
		INFO("Read");
		Vector2u myVector2u = Vector2u(1);
		eng::Visitor visitor = str::StringView("Vector2u = { X = 2, Y = 2 }");
		visitor.Read(strVector2u, myVector2u, Vector2u(1));

		CHECK(myVector2u == Vector2u(2));
	}

	{
		INFO("Read - Default");
		Vector2u myVector2u = Vector2u(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector2u, myVector2u, Vector2u(2));

		CHECK(myVector2u == Vector2u(2));
	}
}

TEST_CASE("eng::Visitor::Vector3f")
{
	{
		INFO("Write");
		Vector3f myVector3f = Vector3f(2.f);
		eng::Visitor visitor;
		visitor.Write(strVector3f, myVector3f);

		str::String string = visitor;
		CHECK(string == "Vector3f = { X = 2.0, Y = 2.0, Z = 2.0 }");
	}

	{
		INFO("Read");
		Vector3f myVector3f = Vector3f(1.f);
		eng::Visitor visitor = str::StringView("Vector3f = { X = 2.0, Y = 2.0, Z = 2.0 }");
		visitor.Read(strVector3f, myVector3f, Vector3f(1.f));

		CHECK(myVector3f == Vector3f(2.f));
	}

	{
		INFO("Read - Default");
		Vector3f myVector3f = Vector3f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector3f, myVector3f, Vector3f(2.f));

		CHECK(myVector3f == Vector3f(2.f));
	}
}

TEST_CASE("eng::Visitor::Vector3i")
{
	{
		INFO("Write");
		Vector3i myVector3i = Vector3i(2);
		eng::Visitor visitor;
		visitor.Write(strVector3i, myVector3i);

		str::String string = visitor;
		CHECK(string == "Vector3i = { X = 2, Y = 2, Z = 2 }");
	}

	{
		INFO("Read");
		Vector3i myVector3i = Vector3i(1);
		eng::Visitor visitor = str::StringView("Vector3i = { X = 2, Y = 2, Z = 2 }");
		visitor.Read(strVector3i, myVector3i, Vector3i(1));

		CHECK(myVector3i == Vector3i(2));
	}

	{
		INFO("Read - Default");
		Vector3i myVector3i = Vector3i(1);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector3i, myVector3i, Vector3i(2));

		CHECK(myVector3i == Vector3i(2));
	}
}

TEST_CASE("eng::Visitor::Vector4f")
{
	{
		INFO("Write");
		Vector4f myVector4f = Vector4f(2.f);
		eng::Visitor visitor;
		visitor.Write(strVector4f, myVector4f);

		str::String string = visitor;
		CHECK(string == "Vector4f = { W = 2.0, X = 2.0, Y = 2.0, Z = 2.0 }");
	}

	{
		INFO("Read");
		Vector4f myVector4f = Vector4f(1.f);
		eng::Visitor visitor = str::StringView("Vector4f = { W = 2.0, X = 2.0, Y = 2.0, Z = 2.0 }");
		visitor.Read(strVector4f, myVector4f, Vector4f(1.f));

		CHECK(myVector4f == Vector4f(2.f));
	}

	{
		INFO("Read - Default");
		Vector4f myVector4f = Vector4f(1.f);
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVector4f, myVector4f, Vector4f(2.f));

		CHECK(myVector4f == Vector4f(2.f));
	}
}

TEST_CASE("eng::Visitor::Array<Enum>")
{
	{
		INFO("Write");
		Array<EInt32> myArray = { EInt32::Max };
		eng::Visitor visitor;
		visitor.Write(strArrayEnum, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Enum>' = [ 2147483647 ]");
	}

	{
		INFO("Read");
		Array<EInt32> myArray;
		eng::Visitor visitor = str::StringView("'Array<Enum>' = [ 2147483647 ]");
		visitor.Read(strArrayEnum, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Array<EInt32> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayEnum, myArray, { EInt32::Max });

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
		visitor.Write(strArrayInt32, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Int32>' = [ 2 ]");
	}

	{
		INFO("Read");
		Array<int32> myArray;
		eng::Visitor visitor = str::StringView("'Array<Int32>' = [ 2 ]");
		visitor.Read(strArrayInt32, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == 2);
	}

	{
		INFO("Read - Default");
		Array<int32> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayInt32, myArray, { 2 });

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == 2);
	}
}

TEST_CASE("eng::Visitor::Array<Vector2f>")
{
	{
		INFO("Write");
		Array<Vector2f> myArray = { Vector2f::One };
		eng::Visitor visitor;
		visitor.Write(strArrayVector2f, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Vector2f>' = [ { X = 1.0, Y = 1.0 } ]");
	}

	{
		INFO("Read");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("[['Array<Vector2f>']]\n X = 1.0\n Y = 1.0");
		visitor.Read(strArrayVector2f, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Vector2f::One);
	}

	{
		INFO("Read - Inline");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("'Array<Vector2f>' = [ { X = 1.0, Y = 1.0 } ]");
		visitor.Read(strArrayVector2f, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0] == Vector2f::One);
	}

	{
		INFO("Read - Multiline");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("[['Array<Vector2f>']]\n X = 1.0\n Y = 1.0\n [['Array<Vector2f>']]\n X = 2.0\n Y = 2.0");
		visitor.Read(strArrayVector2f, myArray, {});

		REQUIRE(myArray.GetCount() == 2);
		CHECK(myArray[0] == Vector2f(1.f));
		CHECK(myArray[1] == Vector2f(2.f));
	}

	{
		INFO("Read - Default");
		Array<Vector2f> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayVector2f, myArray, { Vector2f::One });

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
		visitor.Write(strArrayArrayEnum, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Array<Enum>>' = [ [ 2147483647 ] ]");
	}

	{
		INFO("Read");
		Array<Array<EInt32>> myArray;
		eng::Visitor visitor = str::StringView("'Array<Array<Enum>>' = [ [ 2147483647 ] ]");
		visitor.Read(strArrayArrayEnum, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Array<Array<EInt32>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayArrayEnum, myArray, { { EInt32::Max } });

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
		visitor.Write(strArrayArrayInt32, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Array<Int32>>' = [ [ 2 ] ]");
	}

	{
		INFO("Read - Inline");
		Array<Array<int32>> myArray;
		eng::Visitor visitor = str::StringView("'Array<Array<Int32>>' = [ [ 2 ] ]");
		visitor.Read(strArrayArrayInt32, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == 2);
	}

	{
		INFO("Read - Default");
		Array<Array<int32>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayArrayInt32, myArray, { { 2 } });

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
		visitor.Write(strArrayArrayVector2f, myArray);

		str::String string = visitor;
		CHECK(string == "'Array<Array<Vector2f>>' = [ [ { X = 1.0, Y = 1.0 } ] ]");
	}

	{
		INFO("Read");
		Array<Array<Vector2f>> myArray;
		eng::Visitor visitor = str::StringView("'Array<Array<Vector2f>>' = [ [ { X = 1.0, Y = 1.0 } ] ]");
		visitor.Read(strArrayArrayVector2f, myArray, {});

		REQUIRE(myArray.GetCount() == 1);
		REQUIRE(myArray[0].GetCount() == 1);
		CHECK(myArray[0][0] == Vector2f::One);
	}

	{
		INFO("Read - Default");
		Array<Array<Vector2f>> myArray;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strArrayArrayVector2f, myArray, { { Vector2f::One } });

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
		visitor.Write(strMapEnum, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Enum>']\nA = 2147483647");
	}

	{
		INFO("Read");
		Map<str::String, EInt32> myMap;
		eng::Visitor visitor = str::StringView("['Map<Enum>']\nA = 2147483647");
		visitor.Read(strMapEnum, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == EInt32::Max);
	}

	{
		INFO("Read - Default");
		Map<str::String, EInt32> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMapEnum, myMap, { { "A", EInt32::Max } });

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
		visitor.Write(strMapInt32, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Int32>']\nA = 2");
	}

	{
		INFO("Read");
		Map<str::String, int32> myMap;
		eng::Visitor visitor = str::StringView("['Map<Int32>']\nA = 2");
		visitor.Read(strMapInt32, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == 2);
	}

	{
		INFO("Read - Default");
		Map<str::String, int32> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMapInt32, myMap, { { "A", 2 } });

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
		visitor.Write(strMapVector2f, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Vector2f>']\nA = { X = 1.0, Y = 1.0 }");
	}

	{
		INFO("Read");
		Map<str::String, Vector2f> myMap;
		eng::Visitor visitor = str::StringView("['Map<Vector2f>']\nA = { X = 1.0, Y = 1.0 }");
		visitor.Read(strMapVector2f, myMap, {});

		REQUIRE(myMap.GetCount() == 1);
		CHECK(myMap["A"] == Vector2f::One);
	}

	{
		INFO("Read - Default");
		Map<str::String, Vector2f> myMap;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strMapVector2f, myMap, { { "A", Vector2f::One } });

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
		visitor.Write(strMapMapEnum, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Map<Enum>>'.A]\nB = 2147483647");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, EInt32>> myMap;
		eng::Visitor visitor = str::StringView("['Map<Map<Enum>>'.A]\nB = 2147483647");
		visitor.Read(strMapMapEnum, myMap, {});

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
			visitor.Read(strMapMapEnum, myMap, myDefault);
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
		visitor.Write(strMapMapInt32, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Map<Int32>>'.A]\nB = 2");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, int32>> myMap;
		eng::Visitor visitor = str::StringView("['Map<Map<Int32>>'.A]\nB = 2");
		visitor.Read(strMapMapInt32, myMap, {});

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
			visitor.Read(strMapMapInt32, myMap, myDefault);
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
		visitor.Write(strMapMapVector2f, myMap);

		str::String string = visitor;
		CHECK(string == "['Map<Map<Vector2f>>'.A]\nB = { X = 1.0, Y = 1.0 }");
	}

	{
		INFO("Read");
		Map<str::String, Map<str::String, Vector2f>> myMap;
		eng::Visitor visitor = str::StringView("['Map<Map<Vector2f>>'.A]\nB = { X = 1.0, Y = 1.0 }");
		visitor.Read(strMapMapVector2f, myMap, {});

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
			visitor.Read(strMapMapVector2f, myMap, myDefault);
		}

		REQUIRE(myMap.GetCount() == 1);
		REQUIRE(myMap["A"].GetCount() == 1);
		CHECK(myMap["A"]["B"] == Vector2f::One);
	}
}

TEST_CASE("eng::Visitor::Variant<bool, int32>")
{
	{
		INFO("Write");
		Variant<bool, int32> myVariant = 3;
		eng::Visitor visitor;
		visitor.Write(strVariant, myVariant);

		str::String string = visitor;
		CHECK(string == "[Variant]\nTypeId = 1\nTypeVal = 3");
	}

	{
		INFO("Read");
		Variant<bool, int32> myVariant;
		eng::Visitor visitor = str::StringView("Variant = { TypeId = 1, TypeVal = 3 }");
		visitor.Read(strVariant, myVariant, Variant<bool, int32>{0});

		REQUIRE(std::holds_alternative<int32>(myVariant));
		CHECK(std::get<int32>(myVariant) == 3);
	}

	{
		INFO("Read - Default");
		Variant<bool, int32> myVariant;
		eng::Visitor visitor = str::StringView("");
		visitor.Read(strVariant, myVariant, Variant<bool, int32>{3});

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