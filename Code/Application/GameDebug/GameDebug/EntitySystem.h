#pragma once

#include "Core/Array.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace debug
{
	struct EntityWindowRequest;

	struct EntityWindowComponent final : public ecs::Component<EntityWindowComponent> 
	{ 
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_ComponentsLabel = {};

		ecs::Entity m_ClientEntity = { };
		ecs::Entity m_ServerEntity = { };
	};

	class EntitySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::EntityWindowComponent,
			ecs::NameComponent,
			const debug::EntityWindowRequest>;

		EntitySystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
		imgui::Identifier m_WindowIds = {};
	};
};