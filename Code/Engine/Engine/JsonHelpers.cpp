#include "EnginePCH.h"
#include "Engine/JsonHelpers.h"

#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

bool json::LoadDocument(const str::Path& filepath, json::Document& document)
{
	FILE* file;
	fopen_s(&file, filepath.ToChar(), "r");
	if (!file)
	{
		perror("fopen_s");
		return false;
	}

	constexpr size_t size = 65536;
	char* buffer = new char[size];
	rapidjson::FileReadStream is(file, buffer, size);
	rapidjson::ParseResult result = document.ParseStream(is);
	if (!result)
	{
		printf("JSON parse error: %s (%u)\n", rapidjson::GetParseError_En(result.Code()), (unsigned int)result.Offset());
		fclose(file);
		delete[] buffer;
		return false;
	}

	fclose(file);
	delete[] buffer;
	return true;
}

bool json::SaveDocument(const str::Path& filepath, json::Document& document)
{
	FILE* file;
	fopen_s(&file, filepath.ToChar(), "w");
	if (!file)
	{
		perror("fopen_s");
		return false;
	}

	constexpr size_t size = 65536;
	char* buffer = new char[size];
	rapidjson::FileWriteStream os(file, buffer, size);
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	const bool result = document.Accept(writer);

	fclose(file);
	delete[] buffer;
	return result;
}

bool json::ParseArray(const json::Object& value, const char* member, const json::Callback& callback)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsArray())
	{
		bool result = true;
		for (const auto& child : itr->value.GetArray())
			result |= callback(child);
		return result;
	}
	return false;
}

json::Binary json::ParseBinary(const json::Object& value, const char* member, const json::Binary& _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsString())
		return json::Binary{ itr->value.GetString(), itr->value.GetStringLength() };
	return _default;
}

bool json::ParseBool(const json::Object& value, const char* member, const bool _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsBool())
		return itr->value.GetBool();
	return _default;
}

double json::ParseDouble(const json::Object& value, const char* member, const double _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsNumber())
		return itr->value.GetDouble();
	return _default;
}

float json::ParseFloat(const json::Object& value, const char* member, const float _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsNumber())
		return itr->value.GetFloat();
	return _default;
}

int32 json::ParseInt(const json::Object& value, const char* member, const int32 _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsNumber())
		return itr->value.GetInt();
	return _default;
}

uint32 json::ParseUint(const json::Object& value, const char* member, const uint32 _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsNumber())
		return static_cast<uint32>(itr->value.GetInt());
	return _default;
}

str::String json::ParseString(const json::Object& value, const char* member, const str::String& _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd() && itr->value.IsString())
		return itr->value.GetString();
	return _default;
}

void json::PrintDocument(const json::Document& document)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter writer(buffer);
	document.Accept(writer);
	printf("\"%s\"\n", buffer.GetString());
	printf("=========================\n");
}

void json::PrintMembers(const json::Object& value)
{
	for (auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
		printf("\"%s\" : \"%s\"\n", iter->name.GetString(), iter->value.GetString());
}
