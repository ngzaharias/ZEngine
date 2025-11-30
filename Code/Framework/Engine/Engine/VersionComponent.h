#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

// #release: bump the version number

namespace eng
{
	/// \brief
	struct VersionSingleton final : public ecs::Singleton<VersionSingleton>
	{
		str::String m_Branch = {};
		str::String m_Commit = {};
		str::String m_Version = "beta-0.0.1";
	};
}