#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace crafting
{
	class IngredientTable;
	class RecipeTable;
}

namespace debug::crafting
{
	struct WindowComponent;
	struct WindowEvent;
}

namespace debug::crafting
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::crafting::WindowComponent>
			::Read<
			::crafting::IngredientTable,
			::crafting::RecipeTable,
			debug::crafting::WindowEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};