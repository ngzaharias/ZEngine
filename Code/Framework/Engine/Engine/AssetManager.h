#pragma once

#include "Core/Algorithms.h"
#include "Core/Assert.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Set.h"
#include "Core/StringHelpers.h"
#include "Core/TypeTraits.h"
#include "ECS/Component.h"
#include "Engine/Asset.h"
#include "Engine/TypeInfo.h"
#include "Engine/Visitor.h"

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	class AssetLoader;
	class AssetManager;
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

	struct AssetMethods
	{
		using Initialise = void(eng::Asset&, const eng::AssetLoader&);
		Initialise* m_Initialise = nullptr;
		using Shutdown = void(eng::Asset&, const eng::AssetLoader&);
		Shutdown* m_Shutdown = nullptr;

		using Save = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Save* m_Save = nullptr;
		using Load = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Load* m_Load = nullptr;
		using Import = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Import* m_Import = nullptr;
	};

	struct AssetEntry
	{
		eng::AssetLoader* m_Loader = nullptr;
		eng::AssetMethods m_Methods = {};

		using Load = void(eng::AssetManager&, const str::Path&);
		Load* m_Load = nullptr;
	};

	struct AssetRef
	{
		eng::Asset* m_Asset = nullptr;
		int32 m_Count = 0;
	};

	struct AssetLoadedEvent : public ecs::EventComponent<AssetLoadedEvent> 
	{ 
		eng::Asset* m_Asset = nullptr;
	};

	class AssetManager final
	{
	public:
		static constexpr const char* s_Extension = ".asset";

		using FileMap = Map<str::Guid, eng::AssetFile>;
		using RefMap = Map<str::Guid, AssetRef>;
		using Registry = Map<str::Name, eng::AssetEntry>;
		using TypeMap = Map<str::Name, Set<str::Guid>>;

	public:
		void Initialise();
		void Shutdown();
		
		void Update(ecs::EntityWorld& client, ecs::EntityWorld& server);

		bool HasAsset(const str::Guid& guid);

		template<typename TAsset, typename TLoader, typename... TArgs>
		void RegisterAsset(const str::Name& type, TArgs&&... args);

		template<class TAsset>
		const TAsset* FetchAsset(const str::Guid& guid) const;

		template<class TAsset>
		bool SaveAsset(TAsset& asset, const str::Path& filepath) { return false; }
		template<class TAsset>
		bool ImportAsset(TAsset& asset, const str::Path& filepath) { return false; }

		void LoadAsset(const str::Guid& guid);
		void RequestAsset(const str::Guid& guid);
		void ReleaseAsset(const str::Guid& guid);
		void ReloadAsset(const str::Guid& guid);
		void ReloadAssets();

		auto GetFileMap() const -> const FileMap&;
		auto GetTypeMap() const -> const TypeMap&;

	public:
		// #temp: need to find a better way of discovering assets, AddEntry perhaps ?
		void LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories);

		const eng::AssetFile* GetAssetFile(const str::Guid& guid) const;

	private:
		template<typename TAsset>
		void LoadDeferred(const str::Path filepath);
		template<typename TAsset>
		void LoadImmediate(const str::Path& filepath);

	private:
		template<typename TAsset>
		static void Load(eng::AssetManager& manager, const str::Path& filepath);

		template<typename TAsset, typename TLoader>
		static void InitialiseMethod(eng::Asset& asset, const eng::AssetLoader& loader);
		template<typename TAsset, typename TLoader>
		static void ShutdownMethod(eng::Asset& asset, const eng::AssetLoader& loader);
		template<typename TAsset, typename TLoader>
		static bool SaveMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);
		template<typename TAsset, typename TLoader>
		static bool LoadMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);
		template<typename TAsset, typename TLoader>
		static bool ImportMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);

	private:
		FileMap m_FileMap = { };
		RefMap m_RefMap = { };
		Registry m_Registry = { };
		TypeMap m_TypeMap = { };

		Array<eng::Asset*> m_Loaded = {};
		std::mutex m_Mutex;
	};
}

#include "AssetManager.inl"