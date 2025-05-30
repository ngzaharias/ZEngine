#include "Core/SortHelpers.h"

#include <ctype.h>
#include <stddef.h>

// -*- mode: c; c-file-style: "k&r" -*-
// strnatcmp.c -- Perform 'natural order' comparisons of strings in C.
// Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

namespace natsort
{
	// These are defined as macros to make it easier to adapt this code to
	// different characters types or comparison functions.
	static inline int IsDigit(char a)
	{
		return isdigit((unsigned char)a);
	}


	static inline int IsSpace(char a)
	{
		return isspace((unsigned char)a);
	}


	static inline char ToUpper(char a)
	{
		return toupper((unsigned char)a);
	}


	static int Compare_Right(const str::StringView& a, const str::StringView& b)
	{
		int bias = 0;

		// The longest run of digits wins. That aside, the greatest
		// value wins, but we can't know that it will until we've scanned
		// both numbers to know that they have the same magnitude, so we
		// remember it in BIAS.
		for (int i = 0;; i++)
		{
			const bool ax = i >= a.size() || !IsDigit(a[i]);
			const bool bx = i >= b.size() || !IsDigit(b[i]);
			if (ax && bx)
				return bias;
			if (ax)
				return -1;
			if (bx)
				return +1;

			if (a[i] < b[i])
			{
				if (!bias)
					bias = -1;
			}
			else if (a[i] > b[i])
			{
				if (!bias)
					bias = +1;
			}
			else if (!a[i] && !b[i])
			{
				return bias;
			}
		}

		return 0;
	}


	static int Compare_Left(const str::StringView& a, const str::StringView& b)
	{
		// Compare two left-aligned numbers: the first to have a different value wins.
		for (int i = 0;; i++)
		{
			if (!IsDigit(a[i]) && !IsDigit(b[i]))
				return 0;
			if (!IsDigit(a[i]))
				return -1;
			if (!IsDigit(b[i]))
				return +1;
			if (a[i] < b[i])
				return -1;
			if (a[i] > b[i])
				return +1;
		}

		return 0;
	}


	static int Compare(const str::StringView& a, const str::StringView& b, int fold_case)
	{
		int ai, bi;
		char ca, cb;
		int fractional, result;

		ai = bi = 0;
		while (true)
		{
			// The strings compare the same. Perhaps the caller will want to call strcmp to break the tie.
			if (ai == a.size() && bi == b.size())
				return 0;

			if (a.size() < b.size())
				return -1;
			if (a.size() > b.size())
				return 1;

			ca = a[ai]; cb = b[bi];

			// skip over leading spaces or zeros
			while (IsSpace(ca))
				ca = a[++ai];

			while (IsSpace(cb))
				cb = b[++bi];

			// process run of digits
			if (IsDigit(ca) && IsDigit(cb))
			{
				fractional = (ca == '0' || cb == '0');

				const str::StringView aa = a.substr(ai, a.size() - 1);
				const str::StringView bb = b.substr(bi, b.size() - 1);

				if (fractional)
				{
					if ((result = Compare_Left(aa, bb)) != 0)
						return result;
				}
				else
				{
					if ((result = Compare_Right(aa, bb)) != 0)
						return result;
				}
			}

			if (fold_case)
			{
				ca = ToUpper(ca);
				cb = ToUpper(cb);
			}

			if (ca < cb)
				return -1;

			if (ca > cb)
				return +1;

			++ai; ++bi;
		}
	}

	int Compare(const str::StringView& a, const str::StringView& b)
	{
		return Compare(a, b, 0);
	}


	// Compare, recognizing numeric string and ignoring case.
	int Compare_NoCase(const str::StringView& a, const str::StringView& b)
	{
		return Compare(a, b, 1);
	}
}

int32 sort::AlphaNumeric(const str::StringView& a, const str::StringView& b)
{
	return natsort::Compare_NoCase(a, b);
}