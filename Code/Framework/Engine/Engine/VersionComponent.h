#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

// #release: bump the version number

namespace eng
{
	/// \brief
	struct VersionComponent final : ecs::SingletonComponent<VersionComponent>
	{
		str::String m_Branch = {};
		str::String m_Commit = {};
		str::String m_Version = "beta-0.0.0";
	};
}