#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class PrototypeManager;
	struct InputComponent;
	struct PhysicsComponent;
	struct PrototypeComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
}

namespace editor
{
	struct PhysicsWindowRequestComponent;

	struct PhysicsAssetSaveComponent : public ecs::Component<PhysicsAssetSaveComponent> { };

	struct PhysicsWindowComponent : public ecs::Component<PhysicsWindowComponent>
	{
		ecs::Entity m_Selected = {};

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_InspectorLabel = {};
	};

	class PhysicsEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			const eng::PrototypeManager,
			// components
			editor::PhysicsAssetSaveComponent,
			editor::PhysicsWindowComponent,
			const ecs::NameComponent,
			const eng::InputComponent,
			const eng::PrototypeComponent,
			const eng::RigidDynamicComponent,
			const eng::RigidStaticComponent,
			const editor::PhysicsWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
