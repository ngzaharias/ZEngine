#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/TypeInfo.h"

namespace eng
{
	/// \brief A piece of data such as an image or sound that can't be modified during runtime.
	struct Asset
	{
		Asset() = default;
		virtual ~Asset() = default;

		str::Guid m_Guid = { };
		str::Name m_Name = { };
		str::Name m_Type = { };
	};

	/// \brief Property that marks an assert that it should be loaded off-thread.
	struct DeferredLoad	{ };
}