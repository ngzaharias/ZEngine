#pragma once

#include "Core/Name.h"

namespace ecs
{
	/// \brief Layout of a component within an entity table.
	struct ComponentLayout
	{
		str::Name m_Name = {};
		// The offset of the component within its page.
		uint16 m_Offset = 0;
		// The size of the component in bytes.
		uint16 m_Bytes = 0;

		// The constructor for the component (if it exists).
		void(*m_Constructor)(void*) = nullptr;
		// The copy constructor for the component (if it exists).
		void(*m_Copystructor)(void*, void*) = nullptr;
		// The destructor for the component (if it exists).
		void(*m_Destructor)(void*) = nullptr;
	};
}