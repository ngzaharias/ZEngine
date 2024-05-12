#pragma once

#include <Core/String.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>
#include <ECS/System.h>

namespace eng
{
	struct InputComponent;
	struct NameComponent;
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
			// components
			editor::TransformAssetSaveComponent,
			editor::TransformWindowComponent,
			eng::TransformComponent,
			const editor::TransformWindowRequestComponent,
			const eng::InputComponent,
			const eng::NameComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
