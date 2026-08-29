#include "Core/SortHelpers.h"

#include <ctype.h>
#include <stddef.h>

namespace
{
	inline int IsDigit(char a)
	{
		return std::isdigit((unsigned char)a);
	}

	inline int IsSpace(char a)
	{
		return std::isspace((unsigned char)a);
	}

	inline char ToUpper(char a)
	{
		return std::toupper((unsigned char)a);
	}
}

int32 sort::AlphaNumeric(const str::StringView& a, const str::StringView& b)
{
	uint16 i = 0, j = 0;
	while (i < a.size() && j < b.size())
	{
		if (IsDigit(a[i]) && IsDigit(b[j]))
		{
			// find the end of each number
			uint16 aEnd = i;
			uint16 bEnd = j;

			while (aEnd < a.size() && IsDigit(a[aEnd]))
				++aEnd;
			while (bEnd < b.size() && IsDigit(b[bEnd]))
				++bEnd;

			// compare numbers by length first
			const uint16 aLength = aEnd - i;
			const uint16 bLength = bEnd - j;

			if (aLength != bLength)
				return aLength < bLength ? -1 : +1;

			// same length: compare digits
			for (uint16 k = 0; k < aLength; ++k)
			{
				if (a[i + k] != b[j + k])
					return a[i + k] < b[j + k] ? -1 : +1;
			}

			i = aEnd;
			j = bEnd;
		}
		else
		{
			const char ai = ToUpper(a[i]);
			const char bj = ToUpper(b[j]);
			if (ai != bj)
				return ai < bj ? -1 : +1;

			++i;
			++j;
		}
	}

	return a.size() < b.size() ? -1 : +1;
}