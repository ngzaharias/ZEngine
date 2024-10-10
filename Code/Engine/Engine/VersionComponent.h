#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace eng
{
	/// \brief
	struct VersionComponent final : ecs::SingletonComponent<VersionComponent>
	{
		str::String m_Branch = {};
		str::String m_Commit = {};
		str::String m_Version = "alpha-0.0.3";
	};
}