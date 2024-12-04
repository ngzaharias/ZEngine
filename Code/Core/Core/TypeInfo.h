#pragma once

using int32 = int32_t;

namespace _private
{
	struct TypeIndex
	{
		template<typename Tag>
		[[nodiscard]] static int32 Next() noexcept
		{
			static int32 value = 0;
			return value++;
		}
	};
}

template<typename Type, typename Tag>
int32 ToTypeIndex()
{
	static const int32 value = _private::TypeIndex::Next<Tag>();
	return value;
}

template<typename Type>
const char* ToTypeName()
{
	// #note: MSVC returns "class <TYPE>", "struct <TYPE>" or "<TYPE>"
	// will also include namespaces (including anonymous).
	static const char* value = typeid(Type).name();
	return value;
}
