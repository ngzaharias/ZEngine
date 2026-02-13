#include "EditorPCH.h"
#include "Editor/EditorIconSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EditorIcons.h"
#include "Engine/AssetManager.h"
#include "Engine/Texture2DAsset.h"

void editor::IconSystem::Initialise(World& world)
{
	str::Path filepath = str::Path(str::EPath::Assets, "Textures/T_Icon_Atlas.asset");

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);

	eng::Texture2DAsset asset;
	eng::Texture2DAssetLoader loader;
	loader.Load(asset, visitor);
	loader.Bind(asset);

	using namespace icon;
#define ICON_DEFINITION(name, posx, posy) \
	name.m_TextureId = asset.m_TextureId; \
	name.m_UV0.x = posx / asset.m_Width; \
	name.m_UV0.y = posy / asset.m_Height; \
	name.m_UV1.x = (posx + 32.f) / asset.m_Width; \
	name.m_UV1.y = (posy + 32.f) / asset.m_Height;
#include "Editor/EditorIconList.h"
#undef ICON_DEFINITION

#define ICON_DEFINITION(name, posx, posy) s_List.Append(&name);
#include "Editor/EditorIconList.h"
#undef ICON_DEFINITION
}

void editor::IconSystem::Shutdown(World& world)
{
}