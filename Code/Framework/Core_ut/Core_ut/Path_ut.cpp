#include "CorePCH.h"

#include "Core/Path.h"

#include <filesystem>

#define CLASS_TEST_CASE(name) TEST_CASE("str::Path. " name, "[str::Path]")

CLASS_TEST_CASE("HasExtension.")
{
	CHECK(!str::Path("foo/bar").HasExtension());
	CHECK(!str::Path("foo/bar/").HasExtension());
	CHECK(str::Path("foo/bar.").HasExtension());
	CHECK(str::Path("foo/bar.txt").HasExtension());
}

CLASS_TEST_CASE("IsDirectory.")
{
	CHECK(str::Path("/foo").IsDirectory());
	CHECK(str::Path("foo/").IsDirectory());
	CHECK(!str::Path("bar.txt").IsDirectory());
	CHECK(str::Path("/foo/bar").IsDirectory());
	CHECK(str::Path("foo/bar/").IsDirectory());
	CHECK(!str::Path("foo/bar.txt").IsDirectory());
}

CLASS_TEST_CASE("IsEmpty.")
{
	CHECK(str::Path().IsEmpty());
	CHECK(str::Path("").IsEmpty());
	CHECK(!str::Path("foo/bar.txt").IsEmpty());
}

CLASS_TEST_CASE("GetDirectory.")
{
	CHECK(str::Path("/foo").GetDirectory() == "/foo");
	CHECK(str::Path("foo/").GetDirectory() == "foo/");
	CHECK(str::Path("bar.txt").GetDirectory() == "");
	CHECK(str::Path("foo/bar.txt").GetDirectory() == "foo/");
}

CLASS_TEST_CASE("GetFileExtension.")
{
	CHECK(str::Path("bar.txt").GetFileExtension() == ".txt");
	CHECK(str::Path("foo/bar.txt").GetFileExtension() == ".txt");
}

CLASS_TEST_CASE("GetFileName.")
{
	CHECK(str::Path("bar.txt").GetFileName() == "bar.txt");
	CHECK(str::Path("foo/bar.txt").GetFileName() == "bar.txt");
}

CLASS_TEST_CASE("GetFileNameNoExtension.")
{
	CHECK(str::Path("bar.txt").GetFileNameNoExtension() == "bar");
	CHECK(str::Path("foo/bar.txt").GetFileNameNoExtension() == "bar");
}

CLASS_TEST_CASE("GetParent.")
{
	CHECK(str::Path("/foo").GetParent() == "");
	CHECK(str::Path("foo/").GetParent() == "");
	CHECK(str::Path("bar.txt").GetParent() == "");
	CHECK(str::Path("foo/bar.txt").GetParent() == "foo");
}

CLASS_TEST_CASE("GetStem.")
{
	CHECK(str::Path("/foo").GetStem() == "foo");
	CHECK(str::Path("foo/").GetStem() == "");
	CHECK(str::Path("bar.txt").GetStem() == "bar.txt");
	CHECK(str::Path("foo/bar.txt").GetStem() == "bar.txt");
}

CLASS_TEST_CASE("Clear.")
{
	str::Path path = "foo/bar.txt";
	REQUIRE(!path.IsEmpty());
	path.Clear();
	CHECK(path.IsEmpty());
}
