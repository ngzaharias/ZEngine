#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Singleton.h"

namespace eng::level
{
	/// \brief Contains every available level that can be loaded.
	struct DirectorySingleton final : public ecs::Singleton<DirectorySingleton>
	{
		Map<str::Name, str::Path> m_Levels;
	};
}