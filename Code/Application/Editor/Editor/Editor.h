#pragma once

#include "ImportEditor/ImportManager.h"

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace editor
{
	class Editor final
	{
	public:
		Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld);

		void Register();

		void Initialise();
		void Shutdown();

		void Update(const GameTime& gameTime);

	public:
		ecs::EntityWorld& m_ClientWorld;
		ecs::EntityWorld& m_ServerWorld;

		editor::importer::Manager m_ImportManager;
	};
}