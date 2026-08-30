#pragma once

namespace ecs
{
	/// \brief Layout of a component within an entity table.
	struct ComponentLayout
	{
		// The destructor for the component (if it exists).
		void(*m_Destructor)(void*) = nullptr;
		// The offset of the component within its page.
		uint16 m_Offset = 0;
		// The size of the component in bytes.
		uint16 m_Bytes = 0;
	};
}