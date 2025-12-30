#include "EnginePCH.h"
#include "Engine/MusicAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::MusicAssetLoader::Load(eng::MusicAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();

	visitor.Read(strSourceFile, asset.m_SourceFile, {});

	const str::Path filepath = str::Path(str::EPath::Assets, asset.m_SourceFile);
	asset.m_Buffer = new sf::Music();
	asset.m_Buffer->setLoop(true);
	asset.m_Buffer->setVolume(0.f);
	asset.m_Buffer->openFromFile(filepath.ToChar());
	asset.m_Buffer->play();

	return true;
}

bool eng::MusicAssetLoader::Unload(eng::MusicAsset& asset) const
{
	PROFILE_FUNCTION();

	delete asset.m_Buffer;
	return true;
}