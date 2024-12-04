#pragma once

#include "Core/Array.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"

namespace str
{
	class Address
	{
	public:
		void Push(const str::StringView& string);
		void Pop();

		str::StringView GetFull() const;
		str::StringView GetLast() const;

	private:
		str::String m_Full = { };
		Array<uint32> m_Positions = { };
	};
}