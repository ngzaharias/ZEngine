#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace editor
{
	struct EntitySaveComponent;
	struct EntitySelectSingleton;
	struct EntityWindowComponent;
	struct EntityWindowRequest;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	struct PhysicsComponent;
	struct PrototypeComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace hidden
{
	struct GroupComponent;
	struct ObjectComponent;
}

namespace editor
{
	class EntityEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::AssetManager,
			const eng::InputManager,
			// Components
			ecs::NameComponent,
			editor::EntitySaveComponent,
			editor::EntityWindowComponent,
			const editor::EntityWindowRequest,
			const eng::level::EntityComponent,
			const eng::PrototypeComponent,
			// Singletons
			editor::EntitySelectSingleton,
			editor::settings::LocalSingleton>;

		EntityEditorSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
