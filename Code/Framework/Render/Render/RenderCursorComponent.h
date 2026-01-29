#pragma once

#include "ECS/Component.h"

namespace render
{
	/// \brief Component used for ordering rendering systems.
	/// Read from this to go after Cursor Rendering.
	struct CursorComponent final : public ecs::Component<CursorComponent> { };
}