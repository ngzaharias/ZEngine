#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace eng
{
	/// \brief
	struct VersionComponent final : ecs::SingletonComponent<VersionComponent>
	{
		str::String m_Version = "0.0.0.1";
		str::String m_Commit = {};
	};
}