#include "EnginePCH.h"
#include "Engine/RenderStage_Noesis_Post.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"

void eng::RenderStage_Noesis_Post::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	auto& uiManager = world.WriteResource<eng::UIManager>();
	uiManager.RenderFinish();
}
