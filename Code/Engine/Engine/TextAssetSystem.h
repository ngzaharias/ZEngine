#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct TextAssetComponent;
	struct TextComponent;
}

namespace eng
{
	class TextAssetSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::TextAssetComponent,
			const eng::TextComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}