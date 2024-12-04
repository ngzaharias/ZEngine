#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

#include <vector>

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
	struct InputComponent;
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
	struct EntityWindowRequestComponent;

	struct EntitySaveComponent : public ecs::Component<EntitySaveComponent> { };

	struct EntityWindowComponent : public ecs::Component<EntityWindowComponent>
	{
		int32 m_Identifier = 0;
		ecs::Entity m_Selected = {};

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_InspectorLabel = {};
	};

	class EntityEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			const eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::EntitySaveComponent,
			editor::EntityWindowComponent,
			editor::settings::LocalComponent,
			const editor::EntityWindowRequestComponent,
			const eng::InputComponent,
			const eng::level::EntityComponent,
			const eng::PrototypeComponent>;

		EntityEditor(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
