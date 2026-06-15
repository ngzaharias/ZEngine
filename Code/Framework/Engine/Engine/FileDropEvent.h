#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "ECS/Event.h"

namespace eng
{
	// \brief Holds all file drop requests from the operating system.
	struct FileDropEvent final : public ecs::Event
	{
		Array<str::Path> m_Files = {};
	};
}