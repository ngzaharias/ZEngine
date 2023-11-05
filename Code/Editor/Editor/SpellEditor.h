#pragma once

#include <ECS/Component.h>
#include <ECS/System.h>

#include <GameClient/SpellDefinitions.h>

namespace spell
{
	struct DefinitionMapComponent;
}

namespace editor
{
	struct SpellWindowRequestComponent;

	struct SpellWindowComponent : public ecs::Component<SpellWindowComponent> 
	{ 
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
	};

	class SpellEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// components
			editor::SpellWindowComponent,
			spell::DefinitionMapComponent,
			const editor::SpellWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};