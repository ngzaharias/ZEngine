#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace editor
{
	struct SpriteBatchingComponent final : public ecs::Component<SpriteBatchingComponent>
	{
		/// \brief How many iterations are done along X and Y.
		/// X is iterated again for each iteration of Y.
		Vector2u m_Iterations = Vector2u::One;
		/// \brief Initial position of the batching pass.
		Vector2f m_Initial = Vector2f::Zero;
		/// \brief The stride between each sprite.
		Vector2f m_Stride = Vector2f(128.f);

		/// \brief The format for sprite names.
		str::String m_Format = "SP_Sprite_";
		/// \brief The number of sprites.
		int32 m_Count = 1;

		/// \brief If it should be previewed.
		bool m_IsPreviewing = true;
	};
}
