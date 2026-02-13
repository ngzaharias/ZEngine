#pragma once

#include "Core/Types.h"
#include "Math/Vector.h"

namespace editor
{
	struct Icon
	{
		uint32 m_TextureId = 0;
		Vector2f m_UV0 = {};
		Vector2f m_UV1 = {};
	};
}
