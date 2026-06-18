#pragma once

#include "Core/Array.h"
#include "Core/Optional.h"
#include "Core/Path.h"
#include "Core/Variant.h"
#include "ECS/Component.h"

namespace editor::importer
{
	struct WindowComponent final : public ecs::SoloComponent
	{
		Array<str::Path> m_Files = {};
	};
}
