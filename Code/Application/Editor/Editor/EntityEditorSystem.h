#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

#include <vector>

namespace editor
{
	struct EntitySelectComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace ecs
{
	struct NameComponent;
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

	struct EntitySaveComponent : public ecs::Component<EntitySaveComponent> { };

	struct EntityWindowComponent : public ecs::Component<EntityWindowComponent>
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
			// resources
			const eng::AssetManager,
			const eng::InputManager,
			// components
			ecs::NameComponent,
			editor::EntitySaveComponent,
			editor::EntitySelectComponent,
			editor::EntityWindowComponent,
			editor::settings::LocalComponent,
			const editor::EntityWindowRequest,
			const eng::level::EntityComponent,
			const eng::PrototypeComponent>;

		EntityEditorSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
