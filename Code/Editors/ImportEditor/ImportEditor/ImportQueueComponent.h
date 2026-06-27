#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace editor::importer
{
	struct QueueComponent final : public ecs::SoloComponent
	{
		Array<str::Path> m_Files = {};
	};
}
