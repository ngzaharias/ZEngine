#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng::level
{
	/// \brief Contains every available level that can be loaded.
	struct DirectoryComponent final : public ecs::StaticComponent
	{
		Map<str::Name, str::Path> m_Levels;
	};
}