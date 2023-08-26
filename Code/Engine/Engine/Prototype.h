#pragma once

#include <Core/Guid.h>
#include <Core/Name.h>
#include <Core/Path.h>

namespace eng
{
	struct Prototype
	{
		Prototype() = default;
		virtual ~Prototype() = default;

		str::Guid m_Guid = { };
		str::Name m_Name = { };
		str::Path m_Path = { }; // #todo: remove ?
		str::Name m_Type = { };
	};
}