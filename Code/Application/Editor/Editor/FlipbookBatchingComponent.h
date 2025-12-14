#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace editor
{
	struct FlipbookBatchingComponent final : public ecs::Component<FlipbookBatchingComponent>
	{
		/// \brief How many iterations are done along X and Y.
		/// X is iterated again for each iteration of Y.
		Vector2u m_Iterations = Vector2u(1, 1);
		/// \brief Initial position of the batching pass.
		Vector2f m_Initial = Vector2f(100.f);
		/// \brief The stride between each frame.
		Vector2f m_Stride = Vector2f(100.f);

		/// \brief The size of each frame.
		Vector2f m_Size = Vector2f(100.f);
		/// \brief The number of frames.
		int32 m_Count = 1;

		/// \brief If it should be previewed.
		bool m_IsPreviewing = true;
	};
}
