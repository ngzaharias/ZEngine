#include "CorePCH.h"

#include "Core/Guid.h"

TEST_CASE("str::Guid. Default constructor.")
{
	str::Guid guid;
	CHECK(guid == str::Guid::Unassigned);

	CHECK(guid.m_Data.m_U64[0] == 0);
	CHECK(guid.m_Data.m_U64[1] == 0);

	CHECK(guid.m_Data.m_U32[0] == 0);
	CHECK(guid.m_Data.m_U32[1] == 0);
	CHECK(guid.m_Data.m_U32[2] == 0);
	CHECK(guid.m_Data.m_U32[3] == 0);

	CHECK(guid.m_Data.m_U8[0] == 0);
	CHECK(guid.m_Data.m_U8[1] == 0);
	CHECK(guid.m_Data.m_U8[2] == 0);
	CHECK(guid.m_Data.m_U8[3] == 0);
	CHECK(guid.m_Data.m_U8[4] == 0);
	CHECK(guid.m_Data.m_U8[5] == 0);
	CHECK(guid.m_Data.m_U8[6] == 0);
	CHECK(guid.m_Data.m_U8[7] == 0);
	CHECK(guid.m_Data.m_U8[8] == 0);
	CHECK(guid.m_Data.m_U8[9] == 0);
	CHECK(guid.m_Data.m_U8[10] == 0);
	CHECK(guid.m_Data.m_U8[11] == 0);
	CHECK(guid.m_Data.m_U8[12] == 0);
	CHECK(guid.m_Data.m_U8[13] == 0);
	CHECK(guid.m_Data.m_U8[14] == 0);
	CHECK(guid.m_Data.m_U8[15] == 0);
}

TEST_CASE("str::Guid. Value constructor.")
{
	{
		str::Guid guid({ 0, 0 });
		CHECK(guid == str::Guid::Unassigned);

		CHECK(guid.m_Data.m_U64[0] == 0);
		CHECK(guid.m_Data.m_U64[1] == 0);

		CHECK(guid.m_Data.m_U32[0] == 0);
		CHECK(guid.m_Data.m_U32[1] == 0);
		CHECK(guid.m_Data.m_U32[2] == 0);
		CHECK(guid.m_Data.m_U32[3] == 0);

		CHECK(guid.m_Data.m_U8[0] == 0);
		CHECK(guid.m_Data.m_U8[1] == 0);
		CHECK(guid.m_Data.m_U8[2] == 0);
		CHECK(guid.m_Data.m_U8[3] == 0);
		CHECK(guid.m_Data.m_U8[4] == 0);
		CHECK(guid.m_Data.m_U8[5] == 0);
		CHECK(guid.m_Data.m_U8[6] == 0);
		CHECK(guid.m_Data.m_U8[7] == 0);
		CHECK(guid.m_Data.m_U8[8] == 0);
		CHECK(guid.m_Data.m_U8[9] == 0);
		CHECK(guid.m_Data.m_U8[10] == 0);
		CHECK(guid.m_Data.m_U8[11] == 0);
		CHECK(guid.m_Data.m_U8[12] == 0);
		CHECK(guid.m_Data.m_U8[13] == 0);
		CHECK(guid.m_Data.m_U8[14] == 0);
		CHECK(guid.m_Data.m_U8[15] == 0);
	}

	{
		str::Guid guid({ UINT64_MAX, UINT64_MAX });

		CHECK(guid.m_Data.m_U64[0] == UINT64_MAX);
		CHECK(guid.m_Data.m_U64[1] == UINT64_MAX);

		CHECK(guid.m_Data.m_U32[0] == UINT32_MAX);
		CHECK(guid.m_Data.m_U32[1] == UINT32_MAX);
		CHECK(guid.m_Data.m_U32[2] == UINT32_MAX);
		CHECK(guid.m_Data.m_U32[3] == UINT32_MAX);

		CHECK(guid.m_Data.m_U8[0] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[1] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[2] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[3] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[4] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[5] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[6] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[7] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[8] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[9] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[10] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[11] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[12] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[13] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[14] == UINT8_MAX);
		CHECK(guid.m_Data.m_U8[15] == UINT8_MAX);
	}

	{
		str::Guid guid({ 1, 1 });

		CHECK(guid.m_Data.m_U64[0] == 1);
		CHECK(guid.m_Data.m_U64[1] == 1);

		CHECK(guid.m_Data.m_U32[0] == 1);
		CHECK(guid.m_Data.m_U32[1] == 0);
		CHECK(guid.m_Data.m_U32[2] == 1);
		CHECK(guid.m_Data.m_U32[3] == 0);

		CHECK(guid.m_Data.m_U8[0] == 1);
		CHECK(guid.m_Data.m_U8[1] == 0);
		CHECK(guid.m_Data.m_U8[2] == 0);
		CHECK(guid.m_Data.m_U8[3] == 0);
		CHECK(guid.m_Data.m_U8[4] == 0);
		CHECK(guid.m_Data.m_U8[5] == 0);
		CHECK(guid.m_Data.m_U8[6] == 0);
		CHECK(guid.m_Data.m_U8[7] == 0);
		CHECK(guid.m_Data.m_U8[8] == 1);
		CHECK(guid.m_Data.m_U8[9] == 0);
		CHECK(guid.m_Data.m_U8[10] == 0);
		CHECK(guid.m_Data.m_U8[11] == 0);
		CHECK(guid.m_Data.m_U8[12] == 0);
		CHECK(guid.m_Data.m_U8[13] == 0);
		CHECK(guid.m_Data.m_U8[14] == 0);
		CHECK(guid.m_Data.m_U8[15] == 0);
	}

	{
		str::Guid guid({ 255, 0 });
		CHECK(guid.m_Data.m_U64[0] == 255);
		CHECK(guid.m_Data.m_U64[1] == 0);
	}
}

TEST_CASE("str::Guid. ToString.")
{
	{
		str::String guid = str::Guid::Unassigned.ToString();
		CHECK(guid == "00000000000000000000000000000000");
	}

	{
		str::String guid = str::Guid({ 1, 0 }).ToString();
		CHECK(guid == "00000001000000000000000000000000");
	}

	{
		str::String guid = str::Guid({ 255, 0 }).ToString();
		CHECK(guid == "000000ff000000000000000000000000");
	}
}

TEST_CASE("str::Guid. IsValid.")
{
	CHECK_FALSE(str::Guid::Unassigned.IsValid());
	CHECK(str::Guid({ 1, 0 }).IsValid());
	CHECK(str::Guid({ 255, 0 }).IsValid());

	CHECK_FALSE(str::Guid::Create("00000000000000000000000000000000").IsValid());
	CHECK(str::Guid::Create("01000000000000000000000000000000").IsValid());
	CHECK(str::Guid::Create("FF000000000000000000000000000000").IsValid());
}

TEST_CASE("str::Guid. Create(str::StringView).")
{
	{
		str::Guid guid = str::Guid::Create("00000000000000000000000000000000");
		CHECK(guid == str::Guid::Unassigned);
	}

	{
		str::Guid guid = str::Guid::Create("00000001000000000000000000000000");
		CHECK(guid.m_Data.m_U8[0] == 1);
	}

	{
		str::Guid guid = str::Guid::Create("000000ff000000000000000000000000");
		CHECK(guid.m_Data.m_U8[0] == 255);
	}

	{
		INFO("String case doesn't change the value.");
		str::Guid guidA = str::Guid::Create("000000ff000000000000000000000000");
		str::Guid guidB = str::Guid::Create("000000FF000000000000000000000000");
		CHECK(guidA == guidB);
	}

	{
		INFO("Values outside 0-9 and A-F are set to 0.");
		str::Guid guid = str::Guid::Create("0G0H0I0J000000000000000000000000");
		CHECK(guid.m_Data.m_U8[0] == 0);
		CHECK(guid.m_Data.m_U8[1] == 0);
		CHECK(guid.m_Data.m_U8[2] == 0);
		CHECK(guid.m_Data.m_U8[3] == 0);
	}
}

TEST_CASE("str::Guid. Generate.")
{
	for (int32 i = 0; i < 1000; ++i)
	{
		str::Guid guidA = str::Guid::Generate();
		str::String stringA = guidA.ToString();
		str::Guid guidB = str::Guid::Create(stringA);
		str::String stringb = guidB.ToString();
		CHECK(guidA == guidB);
		CHECK(stringA == stringb);
	}
}
