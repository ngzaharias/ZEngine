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
	struct PrototypeComponent;
	struct TransformComponent;
}

namespace editor
{
	struct TransformWindowRequestComponent;

	struct TransformAssetSaveComponent : public ecs::Component<TransformAssetSaveComponent> { };

	struct TransformWindowComponent : public ecs::Component<TransformWindowComponent>
	{
		ecs::Entity m_Selected = {};

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_InspectorLabel = {};
	};

	class TransformEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			const eng::PrototypeManager,
			// components
			editor::TransformAssetSaveComponent,
			editor::TransformWindowComponent,
			eng::TransformComponent,
			const ecs::NameComponent,
			const editor::TransformWindowRequestComponent,
			const eng::InputComponent,
			const eng::PrototypeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
