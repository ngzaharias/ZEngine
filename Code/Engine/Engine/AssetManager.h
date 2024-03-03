#pragma once

#include <Core/Algorithms.h>
#include <Core/Assert.h>
#include <Core/Guid.h>
#include <Core/Map.h>
#include <Core/Name.h>
#include <Core/StringHelpers.h>
#include <Core/TypeTraits.h>

#include <Engine/Asset.h>
#include <Engine/TypeInfo.h>
#include <Engine/Visitor.h>

namespace eng
{
	class AssetLoader;
	class Visitor;
}

namespace eng
{
	struct AssetFile
	{
		AssetFile(const str::Guid& guid, const str::Path& path, const str::Name& type)
			: m_Guid(guid), m_Path(path), m_Type(type) { }

		str::Guid m_Guid = { };
		str::Path m_Path = { };
		str::Name m_Type = { };
	};

	struct AssetEntry
	{
		using Import = bool(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);
		using Load = bool(eng::Asset* asset, const eng::AssetLoader& loader);
		using Save = bool(eng::Asset* asset, const eng::AssetLoader& loader);

		str::Name m_Type = { };
		eng::AssetLoader* m_Loader = nullptr;

		Import* m_Import = nullptr;
		Load* m_Load = nullptr;
		Save* m_Save = nullptr;
	};

	class AssetManager final
	{
		static constexpr const char* s_Extension = ".asset";

		using FileMap = Map<str::Guid, eng::AssetFile>;
		using EntryMap = Map<TypeId, eng::AssetEntry>;

	public:
		void Initialise();
		void Shutdown();

		template<typename TAsset, typename TLoader, typename... TArgs>
		void RegisterAsset(const str::Name& type, TArgs&&... args);

		template<class TAsset>
		const TAsset* ImportAsset(const str::Path& inputPath, const str::Path& outputPath);
		template<class TAsset>
		const TAsset* LoadAsset(const str::Guid& guid);
		template<class TAsset>
		bool SaveAsset(TAsset& asset, const str::Path& filepath);

	public:
		// #temp: need to find a better way of discovering assets, AddEntry perhaps ?
		void LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories);

		template<typename TAsset, typename TLoader>
		static bool ImportFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);

		template<typename TAsset, typename TLoader>
		static bool LoadFunction(eng::Asset* asset, const eng::AssetLoader& loader);

		template<typename TAsset, typename TLoader>
		static bool SaveFunction(eng::Asset* asset, const eng::AssetLoader& loader);

	private:
		FileMap m_FileMap = { };
		EntryMap m_EntryMap = { };
	};
}

#include "AssetManager.inl"