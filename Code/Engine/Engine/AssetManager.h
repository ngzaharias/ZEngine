#pragma once

#include "Core/Algorithms.h"
#include "Core/Assert.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Set.h"
#include "Core/StringHelpers.h"
#include "Core/TypeTraits.h"
#include "Engine/Asset.h"
#include "Engine/TypeInfo.h"
#include "Engine/Visitor.h"

namespace eng
{
	class AssetLoader;
	class Visitor;
}

namespace eng
{
	struct AssetFile
	{
		str::Guid m_Guid = { };
		str::Name m_Name = { };
		str::Path m_Path = { };
		str::Name m_Type = { };
	};

	struct AssetEntry
	{
		using Import = bool(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);
		using Load = bool(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);
		using Save = bool(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);

		str::Name m_Type = { };
		eng::AssetLoader* m_Loader = nullptr;

		Save* m_Save = nullptr;
		Load* m_Load = nullptr;
		Import* m_Import = nullptr;
	};

	struct AssetRef
	{
		const eng::Asset* m_Asset = nullptr;

		int32 m_Count = 0;
	};

	class AssetManager final
	{
	public:
		static constexpr const char* s_Extension = ".asset";

		using FileMap = Map<str::Guid, eng::AssetFile>;
		using RefMap = Map<str::Guid, AssetRef>;
		using Registry = Map<TypeId, eng::AssetEntry>;
		using TypeMap = Map<str::Name, Set<str::Guid>>;

	public:
		void Initialise();
		void Shutdown();

		template<typename TAsset, typename TLoader, typename... TArgs>
		void RegisterAsset(const str::Name& type, TArgs&&... args);

		template<class TAsset>
		bool SaveAsset(TAsset& asset, const str::Path& filepath);
		template<class TAsset>
		bool LoadAsset(TAsset& asset, const str::Path& filepath);
		template<class TAsset>
		bool ImportAsset(TAsset& asset, const str::Path& filepath);

		template<class TAsset>
		const TAsset* FetchAsset(const str::Guid& guid) const;
		template<class TAsset>
		void RequestAsset(const str::Guid& guid);
		template<class TAsset>
		void ReleaseAsset(const str::Guid& guid);

		// #temp: need to find a better way of discovering assets, AddEntry perhaps ?
		void LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories);

	public:
		const eng::AssetFile* GetAssetFile(const str::Guid& guid) const;

		void ReloadAssets();

	private:
		template<class TAsset>
		const TAsset* LoadAsset(const str::Guid& guid);

	public:
		template<typename TAsset, typename TLoader>
		static bool SaveFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);
		template<typename TAsset, typename TLoader>
		static bool LoadFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);
		template<typename TAsset, typename TLoader>
		static bool ImportFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath);

	public:
		FileMap m_FileMap = { };
		RefMap m_RefMap = { };
		Registry m_Registry = { };
		TypeMap m_TypeMap = { };
	};
}

#include "AssetManager.inl"