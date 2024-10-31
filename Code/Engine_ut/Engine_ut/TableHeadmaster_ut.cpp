#include <Catch2/catch.hpp>

#include "Core/Guid.h"
#include "Engine/FileHelpers.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TableManager.h"
#include "Engine/Visitor.h"

namespace eng
{
	struct MyObject
	{
		int32 m_Int32 = 0;
	};

	class GuidKeyTable : public eng::TableManager<str::Guid, MyObject> {};
	class NameKeyTable : public eng::TableManager<str::Name, MyObject> {};
	class StringKeyTable : public eng::TableManager<str::String, MyObject> {};
	class ShutdownTable : public eng::TableManager<str::Guid, MyObject> 
	{
	public:
		ShutdownTable(int32& value) : m_Value(value) {}
		~ShutdownTable() { m_Value++; }
		int32& m_Value;
	};

	const str::Guid strGuid1 = str::Guid::Create("38f8294fbc664644a48a951a4595deb4");
	const str::Guid strGuid2 = str::Guid::Create("99b96b9a87dd4e5cb51193fffbfbe051");
	const str::Guid strGuid3 = str::Guid::Create("e27cf062e71e41799ea37ddcc57e447f");
	const str::Guid strGuid4 = str::Guid::Create("8b348b2885f24664a9dfec564a276e61");

	const str::Name strName1 = str::Name::Create("First");
	const str::Name strName2 = str::Name::Create("Second");
	const str::Name strName3 = str::Name::Create("Third");
	const str::Name strName4 = str::Name::Create("Fourth");

	const str::String strString1 = "First";
	const str::String strString2 = "Second";
	const str::String strString3 = "Third";
	const str::String strString4 = "Fourth";
}

template<>
void eng::Visitor::ReadCustom(eng::MyObject& value) const
{
	Read("m_Int32", value.m_Int32, 0);
}

namespace eng
{
	struct RAIIHelper
	{
		RAIIHelper()
		{
			const str::Path root = eng::GetCurrentFilepath().GetParent();
			m_AssetPath = root.GetParent();
			m_AssetPath = m_AssetPath.GetParent();
			m_AssetPath = m_AssetPath.GetParent();
			m_AssetPath += "\\Code\\";
			m_AssetPath += root.GetStem();
			m_AssetPath += "\\Assets\\TableHeadmaster_ut";
		}

		~RAIIHelper()
		{
			m_Headmaster.Shutdown();
		}

		str::Path m_AssetPath = {};
		eng::TableHeadmaster m_Headmaster = {};
	};

	TEST_CASE("eng::TableHeadmaster. Guid as a key for the table.")
	{
		RAIIHelper raiihelper;
		auto& headmaster = raiihelper.m_Headmaster;
		headmaster.Register<GuidKeyTable>("GuidKeyTable");
		headmaster.Initialise(raiihelper.m_AssetPath);

		auto& manager = headmaster.GetManager<GuidKeyTable>();
		auto& objects = manager.GetObjectMap();
		CHECK(objects.GetCount() == 4);
		REQUIRE(objects.Contains(strGuid1));
		REQUIRE(objects.Contains(strGuid2));
		REQUIRE(objects.Contains(strGuid3));
		REQUIRE(objects.Contains(strGuid4));
		CHECK(objects.Get(strGuid1).m_Int32 == 1);
		CHECK(objects.Get(strGuid2).m_Int32 == 2);
		CHECK(objects.Get(strGuid3).m_Int32 == 3);
		CHECK(objects.Get(strGuid4).m_Int32 == 4);
	}

	TEST_CASE("eng::TableHeadmaster. Name as a key for the table.")
	{
		RAIIHelper raiihelper;
		auto& headmaster = raiihelper.m_Headmaster;
		headmaster.Register<NameKeyTable>("NameKeyTable");
		headmaster.Initialise(raiihelper.m_AssetPath);

		auto& manager = headmaster.GetManager<NameKeyTable>();
		auto& objects = manager.GetObjectMap();
		CHECK(objects.GetCount() == 4);
		REQUIRE(objects.Contains(strName1));
		REQUIRE(objects.Contains(strName2));
		REQUIRE(objects.Contains(strName3));
		REQUIRE(objects.Contains(strName4));
		CHECK(objects.Get(strName1).m_Int32 == 1);
		CHECK(objects.Get(strName2).m_Int32 == 2);
		CHECK(objects.Get(strName3).m_Int32 == 3);
		CHECK(objects.Get(strName4).m_Int32 == 4);
	}

	TEST_CASE("eng::TableHeadmaster. String as a key for the table.")
	{
		RAIIHelper raiihelper;
		auto& headmaster = raiihelper.m_Headmaster;
		headmaster.Register<StringKeyTable>("StringKeyTable");
		headmaster.Initialise(raiihelper.m_AssetPath);

		auto& manager = headmaster.GetManager<StringKeyTable>();
		auto& objects = manager.GetObjectMap();
		CHECK(objects.GetCount() == 4);
		REQUIRE(objects.Contains(strString1));
		REQUIRE(objects.Contains(strString2));
		REQUIRE(objects.Contains(strString3));
		REQUIRE(objects.Contains(strString4));
		CHECK(objects.Get(strString1).m_Int32 == 1);
		CHECK(objects.Get(strString2).m_Int32 == 2);
		CHECK(objects.Get(strString3).m_Int32 == 3);
		CHECK(objects.Get(strString4).m_Int32 == 4);
	}

	TEST_CASE("eng::TableHeadmaster. Tables are cleaned up during Shutdown.")
	{
		int32 value = 0;
		RAIIHelper raiihelper;
		auto& headmaster = raiihelper.m_Headmaster;
		headmaster.Register<ShutdownTable>("ShutdownTable", value);
		headmaster.Shutdown();

		CHECK(value == 1);
	}

	TEST_CASE("eng::TableHeadmaster. Tables aren't cleaned up if Shutdown isn't called.")
	{
		int32 value = 0;
		RAIIHelper raiihelper;
		auto& headmaster = raiihelper.m_Headmaster;
		headmaster.Register<ShutdownTable>("ShutdownTable", value);
		headmaster.Initialise(raiihelper.m_AssetPath);

		CHECK(value == 0);
	}
}