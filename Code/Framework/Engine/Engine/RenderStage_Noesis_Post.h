#pragma once

#include "Engine/RenderStage.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class UIManager;
}

namespace eng
{
	class RenderStage_Noesis_Post final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<eng::UIManager>;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}