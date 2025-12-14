#pragma once

#include "Core/Array.h"
#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class RenderStage;
	class UIManager;
	class WindowManager;
}

namespace eng
{
	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::WindowManager>;

		RenderSystem(ecs::EntityWorld& entityWorld);
		~RenderSystem();

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

		template<typename TRenderStage, typename... TArgs>
		void RegisterStage(TArgs&&... args);

	private:
		ecs::EntityWorld& m_EntityWorld;
		Array<RenderStage*> m_RenderStages;
	};
};

#include "RenderSystem.inl"