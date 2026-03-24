#include "GameState/GameStateEditorSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditorComponent.h"

void gamestate::EditorSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_EDITOR
	world.AddComponent<gamestate::EditorComponent>();
#endif
}