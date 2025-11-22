#include "EnginePCH.h"
#include "Engine/AssetManager.h"

#include "Core/StringHelpers.h"
#include "ECS/EntityWorld.h"
#include "Engine/AssetLoader.h"

#include <filesystem>

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strName = "m_Name";
	const str::StringView strType = "m_Type";
}

void eng::AssetManager::Initialise()
{
	const str::Path assets = str::GetPath(str::EPath::Assets);
	LoadFilepath(assets, true);
}

void eng::AssetManager::Shutdown()
{
}

void eng::AssetManager::Update()
{
	if (!m_Loaded.IsEmpty())
	{
		m_Mutex.lock();
		Array<eng::Asset*> loaded;
		std::swap(loaded, m_Loaded);
		m_Mutex.unlock();

		for (eng::Asset* asset : loaded)
		{
			const eng::AssetEntry& entry = m_Registry.Get(asset->m_Type);

			// #note: don't lock mutex before calling initialise since asset loaders can request other assets
			if (entry.m_Methods.m_Initialise)
				entry.m_Methods.m_Initialise(*asset, *entry.m_Loader);

			m_RefMap[asset->m_Guid].m_Asset = asset;
		}
	}
}

bool eng::AssetManager::HasAsset(const str::Guid& guid)
{
	return enumerate::Contains(m_FileMap, guid);
}

void eng::AssetManager::RequestAsset(const str::Guid& guid)
{
	eng::AssetRef& ref = m_RefMap[guid];

	ref.m_Count++;
	if (ref.m_Count == 1)
	{
		Z_PANIC(!ref.m_Asset, "Reference asset already exists! Guid '{}'", guid.ToString());
		LoadAsset(guid);
	}
}

void eng::AssetManager::ReleaseAsset(const str::Guid& guid)
{
	eng::AssetRef& ref = m_RefMap.Get(guid);

	ref.m_Count--;
	if (ref.m_Count == 0)
	{
		UnloadAsset(guid);
	}
	else
	{
		Z_PANIC(ref.m_Count > 0, "Reference count dropped below 0! Guid '{}'", guid.ToString());
	}
}

void eng::AssetManager::ReloadAsset(const str::Guid& guid)
{
	const auto file = m_FileMap.Find(guid);
	const auto ref = m_RefMap.Find(guid);
	if (file != m_FileMap.end() && ref != m_RefMap.end())
	{
		const eng::AssetEntry& entry = m_Registry.Get(file->second.m_Type);
		if (entry.m_Methods.m_Shutdown)
			entry.m_Methods.m_Shutdown(*ref->second.m_Asset, *entry.m_Loader);

		ref->second.m_Asset = nullptr;

		entry.m_Load(*this, file->second.m_Path);
	}
}

void eng::AssetManager::ReloadAssets()
{
	for (auto& [guid, ref] : m_RefMap)
		ReloadAsset(guid);
}

auto eng::AssetManager::GetFileMap() const -> const FileMap&
{
	return m_FileMap;
}

auto eng::AssetManager::GetTypeMap() const -> const TypeMap&
{
	return m_TypeMap;
}

void eng::AssetManager::LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories)
{
	if (filepath.GetFileExtension() == s_Extension)
	{
		// #todo: peak the guid and type
		eng::Visitor visitor;
		if (visitor.LoadFromFile(filepath))
		{
			eng::AssetFile assetFile;
			assetFile.m_Path = filepath;
			visitor.Read(strGuid, assetFile.m_Guid, {});
			visitor.Read(strName, assetFile.m_Name, {});
			visitor.Read(strType, assetFile.m_Type, {});
			m_FileMap[assetFile.m_Guid] = assetFile;
			m_TypeMap[assetFile.m_Type].Add(assetFile.m_Guid);
		}
	}
	else if (filepath.IsDirectory())
	{
		str::Path subpath;
		std::filesystem::path folderPath = filepath.ToChar();
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(folderPath))
		{
			subpath = entry.path().string();
			LoadFilepath(subpath, canSearchSubdirectories);
		}
	}
}

const eng::AssetFile* eng::AssetManager::GetAssetFile(const str::Guid& guid) const
{
	const auto file = m_FileMap.Find(guid);
	return file != m_FileMap.end()
		? &file->second
		: nullptr;
}

void eng::AssetManager::LoadAsset(const str::Guid& guid)
{
	const auto file = m_FileMap.Find(guid);
	if (file == m_FileMap.end())
		return;

	const eng::AssetEntry& entry = m_Registry.Get(file->second.m_Type);
	entry.m_Load(*this, file->second.m_Path);
}

void eng::AssetManager::UnloadAsset(const str::Guid& guid)
{
	eng::AssetRef& ref = m_RefMap.Get(guid);
	eng::Asset* asset = ref.m_Asset;
	const eng::AssetEntry& entry = m_Registry.Get(asset->m_Type);
	if (entry.m_Methods.m_Shutdown)
		entry.m_Methods.m_Shutdown(*asset, *entry.m_Loader);

	delete asset;
	m_RefMap.Remove(guid);
}