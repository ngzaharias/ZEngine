#pragma once

#include "Core/Array.h"
#include "ECS/System.h"

namespace glfw
{
	class Window;
}

namespace eng
{
	class RenderStage;
	struct FrameBufferComponent;
	struct LinesComponent;

	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		RenderSystem(ecs::EntityWorld& entityWorld);
		~RenderSystem();

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_EntityWorld;

		Array<RenderStage*> m_RenderStages;
	};
};