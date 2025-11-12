#pragma once

#include "Core/Array.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/AssetManager.h"

#include <mutex>

namespace eng
{
	class AssetManager;
}

namespace editor
{
	struct Create
	{
		str::Path m_Path = {};
	};

	struct Delete
	{
		str::Path m_Path = {};
	};

	struct Modify
	{
		str::Path m_Path = {};
	};

	struct Rename
	{
		str::Path m_Old = {};
		str::Path m_New = {};
	};

	using Command = Variant<Create, Delete, Modify, Rename>;

	class AssetReloadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			eng::AssetManager>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void Execute();

	private:
		Array<Command> m_Commands = {};
		std::mutex m_Mutex;
	};
}
