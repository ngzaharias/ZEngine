#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

#include <vector>

namespace ecs
{
	struct NameComponent;
}

namespace editor
{
	struct EntitySelectSingleton;
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
	struct EntityWindowRequest;

	struct EntitySaveComponent final : public ecs::Component<EntitySaveComponent> { };

	struct EntityWindowComponent final : public ecs::Component<EntityWindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_InspectorLabel = {};
	};

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
