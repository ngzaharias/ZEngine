#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace debug
{
	struct ShapeWindowComponent;
	struct ShapeWindowRequest;

	class ShapeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::ShapeWindowComponent,
			ecs::NameComponent,
			const debug::ShapeWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}