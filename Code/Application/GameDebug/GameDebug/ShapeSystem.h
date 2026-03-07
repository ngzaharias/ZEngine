#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace debug
{
	struct ShapeWindowComponent;
	struct ShapeWindowEvent;

	class ShapeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::ShapeWindowComponent,
			ecs::NameComponent>
			::Read<
			debug::ShapeWindowEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}