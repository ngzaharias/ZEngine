#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace dbg
{
	struct ShapeWindowComponent;
	struct ShapeWindowRequest;

	class ShapeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::ShapeWindowComponent,
			ecs::NameComponent,
			const dbg::ShapeWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}