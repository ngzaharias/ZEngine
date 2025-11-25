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
	struct NameComponent;
}

namespace dbg
{
	struct EntityWindowRequest;

	struct EntityWindowComponent : public ecs::Component<EntityWindowComponent> 
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
			dbg::EntityWindowComponent,
			ecs::NameComponent,
			const dbg::EntityWindowRequest>;

		EntitySystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;
		imgui::Identifier m_WindowIds = {};
	};
};