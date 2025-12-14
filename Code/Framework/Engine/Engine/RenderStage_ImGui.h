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
	class RenderStage_ImGui final : public eng::RenderStage
	{
	public:
		void Render(ecs::EntityWorld& entityWorld) override;
	};
}