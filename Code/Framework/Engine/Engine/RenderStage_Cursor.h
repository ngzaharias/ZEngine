#pragma once

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"
#include "Math/Vector.h"

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
}

namespace eng
{
	class RenderStage_Cursor final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::InputManager,
			const eng::WindowManager>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;
	};
}