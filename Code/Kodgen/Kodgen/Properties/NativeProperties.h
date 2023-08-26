/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

namespace kodgen
{
	class NativeProperties
	{
		public:
			/** Property used to automatically parse all nested entities without having to annotate them. */
			inline static std::string const	parseAllNestedProperty	= "kodgen::ParseAllNested";

			NativeProperties()	= delete;
			~NativeProperties()	= delete;
	};
}