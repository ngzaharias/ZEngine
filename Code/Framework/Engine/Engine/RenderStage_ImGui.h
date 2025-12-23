#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

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