#include <Catch2/catch.hpp>

#include "Core/Path.h"

#include <filesystem>

TEST_CASE("str::Path")
{
	{
		INFO("HasExtension");
		CHECK(!str::Path("foo/bar").HasExtension());
		CHECK(!str::Path("foo/bar/").HasExtension());
		CHECK(str::Path("foo/bar.").HasExtension());
		CHECK(str::Path("foo/bar.txt").HasExtension());
	}

	{
		INFO("IsDirectory");
		CHECK(str::Path("/foo").IsDirectory());
		CHECK(str::Path("foo/").IsDirectory());
		CHECK(!str::Path("bar.txt").IsDirectory());
		CHECK(str::Path("/foo/bar").IsDirectory());
		CHECK(str::Path("foo/bar/").IsDirectory());
		CHECK(!str::Path("foo/bar.txt").IsDirectory());
	}

	{
		INFO("IsEmpty");
		CHECK(str::Path().IsEmpty());
		CHECK(str::Path("").IsEmpty());
		CHECK(!str::Path("foo/bar.txt").IsEmpty());
	}

	{
		INFO("GetDirectory");
		CHECK(str::Path("/foo").GetDirectory() == "/foo");
		CHECK(str::Path("foo/").GetDirectory() == "foo/");
		CHECK(str::Path("bar.txt").GetDirectory() == "");
		CHECK(str::Path("foo/bar.txt").GetDirectory() == "foo/");
	}

	{
		INFO("GetFileExtension");
		CHECK(str::Path("bar.txt").GetFileExtension() == ".txt");
		CHECK(str::Path("foo/bar.txt").GetFileExtension() == ".txt");
	}

	{
		INFO("GetFileName");
		CHECK(str::Path("bar.txt").GetFileName() == "bar.txt");
		CHECK(str::Path("foo/bar.txt").GetFileName() == "bar.txt");
	}

	{
		INFO("GetFileNameNoExtension");
		CHECK(str::Path("bar.txt").GetFileNameNoExtension() == "bar");
		CHECK(str::Path("foo/bar.txt").GetFileNameNoExtension() == "bar");
	}

	{
		INFO("GetParent");
		CHECK(str::Path("/foo").GetParent() == "");
		CHECK(str::Path("foo/").GetParent() == "");
		CHECK(str::Path("bar.txt").GetParent() == "");
		CHECK(str::Path("foo/bar.txt").GetParent() == "foo");
	}

	{
		INFO("GetStem");
		CHECK(str::Path("/foo").GetStem() == "foo");
		CHECK(str::Path("foo/").GetStem() == "");
		CHECK(str::Path("bar.txt").GetStem() == "bar.txt");
		CHECK(str::Path("foo/bar.txt").GetStem() == "bar.txt");
	}

	{
		INFO("Clear");
		str::Path path = "foo/bar.txt";
		REQUIRE(!path.IsEmpty());
		path.Clear();
		CHECK(path.IsEmpty());
	}
}