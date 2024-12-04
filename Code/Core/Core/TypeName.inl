#pragma once

#include "Core/StringHelpers.h"
#include "Core/TypeInfo.h"

template<class Type>
const str::StringView TypeName<Type>::m_NoNamespace = TypeName<Type>::NoNamespace();
template<class Type>
const str::StringView TypeName<Type>::m_WithNamespace = TypeName<Type>::WithNamespace();

template<class Type>
str::StringView TypeName<Type>::NoNamespace()
{
	static bool isRegistered = false;
	if (!isRegistered)
	{
		str::StringView value = ToTypeName<Type>();

		// class
		{
			constexpr str::StringView substring = "class ";
			const auto find = value.find(substring);
			if (find != std::string::npos)
				value = value.substr(find + substring.size());
		}

		// struct
		{
			constexpr str::StringView substring = "struct ";
			const auto find = value.find(substring);
			if (find != std::string::npos)
				value = value.substr(find + substring.size());
		}

		// namespace
		{
			constexpr str::StringView substring = "::";
			const auto find = value.rfind(substring);
			if (find != std::string::npos)
				value = value.substr(find + substring.size());
		}

		return value;
	}

	return m_NoNamespace;
}

template<class Type>
str::StringView TypeName<Type>::WithNamespace()
{
	static bool isRegistered = false;
	if (!isRegistered)
	{
		str::StringView value = ToTypeName<Type>();

		// class
		{
			constexpr str::StringView substring = "class ";
			const auto find = value.find(substring);
			if (find != std::string::npos)
				value = value.substr(find + substring.size());
		}

		// struct
		{
			constexpr str::StringView substring = "struct ";
			const auto find = value.find(substring);
			if (find != std::string::npos)
				value = value.substr(find + substring.size());
		}

		return value;
	}

	return m_WithNamespace;
}