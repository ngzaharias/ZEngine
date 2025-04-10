#include "CorePCH.h"

#include "Core/Path.h"

#include <filesystem>

TEST_CASE("str::Path. HasExtension.")
{
	CHECK(!str::Path("foo/bar").HasExtension());
	CHECK(!str::Path("foo/bar/").HasExtension());
	CHECK(str::Path("foo/bar.").HasExtension());
	CHECK(str::Path("foo/bar.txt").HasExtension());
}

TEST_CASE("str::Path. IsDirectory.")
{
	CHECK(str::Path("/foo").IsDirectory());
	CHECK(str::Path("foo/").IsDirectory());
	CHECK(!str::Path("bar.txt").IsDirectory());
	CHECK(str::Path("/foo/bar").IsDirectory());
	CHECK(str::Path("foo/bar/").IsDirectory());
	CHECK(!str::Path("foo/bar.txt").IsDirectory());
}

TEST_CASE("str::Path. IsEmpty.")
{
	CHECK(str::Path().IsEmpty());
	CHECK(str::Path("").IsEmpty());
	CHECK(!str::Path("foo/bar.txt").IsEmpty());
}

TEST_CASE("str::Path. GetDirectory.")
{
	CHECK(str::Path("/foo").GetDirectory() == "/foo");
	CHECK(str::Path("foo/").GetDirectory() == "foo/");
	CHECK(str::Path("bar.txt").GetDirectory() == "");
	CHECK(str::Path("foo/bar.txt").GetDirectory() == "foo/");
}

TEST_CASE("str::Path. GetFileExtension.")
{
	CHECK(str::Path("bar.txt").GetFileExtension() == ".txt");
	CHECK(str::Path("foo/bar.txt").GetFileExtension() == ".txt");
}

TEST_CASE("str::Path. GetFileName.")
{
	CHECK(str::Path("bar.txt").GetFileName() == "bar.txt");
	CHECK(str::Path("foo/bar.txt").GetFileName() == "bar.txt");
}

TEST_CASE("str::Path. GetFileNameNoExtension.")
{
	CHECK(str::Path("bar.txt").GetFileNameNoExtension() == "bar");
	CHECK(str::Path("foo/bar.txt").GetFileNameNoExtension() == "bar");
}

TEST_CASE("str::Path. GetParent.")
{
	CHECK(str::Path("/foo").GetParent() == "");
	CHECK(str::Path("foo/").GetParent() == "");
	CHECK(str::Path("bar.txt").GetParent() == "");
	CHECK(str::Path("foo/bar.txt").GetParent() == "foo");
}

TEST_CASE("str::Path. GetStem.")
{
	CHECK(str::Path("/foo").GetStem() == "foo");
	CHECK(str::Path("foo/").GetStem() == "");
	CHECK(str::Path("bar.txt").GetStem() == "bar.txt");
	CHECK(str::Path("foo/bar.txt").GetStem() == "bar.txt");
}

TEST_CASE("str::Path. Clear.")
{
	str::Path path = "foo/bar.txt";
	REQUIRE(!path.IsEmpty());
	path.Clear();
	CHECK(path.IsEmpty());
}
