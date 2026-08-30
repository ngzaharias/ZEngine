#pragma once

namespace ecs
{
	/// \brief Stores the components for all entities that fit within a single page.
	struct EntityPage
	{
		// Data of all the components.
		char* m_Data = nullptr;
	};
}
