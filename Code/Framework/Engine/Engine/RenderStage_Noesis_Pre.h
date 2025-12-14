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