#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace glfw
{
	class Window;
}

namespace eng
{
	class AssetManager;
	class RenderStage;

	struct FrameBufferComponent;
	struct LinesComponent;

	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::FrameBufferComponent,
			eng::LinesComponent>;

		RenderSystem(
			ecs::EntityWorld& entityWorld,
			glfw::Window& window);
		~RenderSystem();

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_EntityWorld;
		glfw::Window& m_Window;

		Array<RenderStage*> m_RenderStages;
	};
};