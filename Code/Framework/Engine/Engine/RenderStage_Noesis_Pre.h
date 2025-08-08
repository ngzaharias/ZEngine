#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class UIManager;
	class WindowManager;
}

namespace eng
{
	class RenderStage_Noesis_Pre final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::UIManager,
			eng::WindowManager>;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}