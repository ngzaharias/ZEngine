#pragma once

#include "Core/Array.h"
#include "Core/Hash.h"
#include "Core/Map.h"
#include "Core/String.h"
#include "Core/StringView.h"

namespace str
{
	class NameTable final
	{
	public:
		static NameTable& Instance();
		static inline NameTable* s_Instance = nullptr;

		NameTable();
		~NameTable();

		str::Hash Register(const str::StringView& string);

		const str::String& Retrieve(const str::Hash& hash) const;

	private:
		Map<str::Hash, str::String> m_Values;
	};
}