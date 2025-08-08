#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

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