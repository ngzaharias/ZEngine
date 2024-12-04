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
	struct ShapeWindowRequestComponent;

	class ShapeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::ShapeWindowComponent,
			ecs::NameComponent,
			const dbg::ShapeWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}