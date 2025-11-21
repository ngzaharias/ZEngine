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
		
		void Update();

		/// \brief Register an class/struct as an asset.
		template<typename TAsset, typename TLoader, typename... TArgs>
		void RegisterAsset(const str::Name& type, TArgs&&... args);

		/// \brief Checks if the asset exists, but doesn't check if it is loaded.
		bool HasAsset(const str::Guid& guid);
		/// \brief Increases the ref count of an asset. If it is the first call, will schedule the asset to be loaded.
		void RequestAsset(const str::Guid& guid);
		/// \brief Decreases the ref count of an asset. If it is the last call, will schedule the asset to be unloaded.
		void ReleaseAsset(const str::Guid& guid);
		/// \brief Unloads and then loads the asset without changing the ref count.
		void ReloadAsset(const str::Guid& guid);
		/// \brief Unloads and then loads all assets without changing the ref counts.
		void ReloadAssets();

		/// \brief Tries to fetch a non-const asset.
		/// Will return nullptr if the asset isn't loaded.
		template<class TAsset>
		TAsset* WriteAsset(const str::Guid& guid);
		/// \brief Tries to fetch a const asset.
		/// Will return nullptr if the asset isn't loaded.
		template<class TAsset>
		const TAsset* ReadAsset(const str::Guid& guid) const;

		template<class TAsset>
		bool SaveAsset(TAsset& asset, const str::Path& filepath) { return false; }
		template<class TAsset>
		bool ImportAsset(TAsset& asset, const str::Path& filepath) { return false; }

		auto GetFileMap() const -> const FileMap&;
		auto GetTypeMap() const -> const TypeMap&;

		// #temp: need to find a better way of discovering assets, AddEntry perhaps ?
		void LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories);

		const eng::AssetFile* GetAssetFile(const str::Guid& guid) const;

	private:
		/// \brief Creates and loads the asset.
		void LoadAsset(const str::Guid& guid);
		/// \brief Unloads and destroys the asset.
		void UnloadAsset(const str::Guid& guid);

		/// \brief Loads the asset in a different thread.
		template<typename TAsset>
		void LoadDeferred(const str::Path filepath);
		/// \brief Loads the asset immediately on the current thread.
		template<typename TAsset>
		void LoadImmediate(const str::Path& filepath);

	private:
		/// \brief Function that can be used to load an asset deferred or immediate based on the asset type.
		/// The function is bound to the asset entry when it is first registered so that the caller doesn't need the template.
		template<typename TAsset>
		static void ScheduleLoad(eng::AssetManager& manager, const str::Path& filepath);

		/// \brief Function that can be used to initialise an asset after it has been loaded.
		template<typename TAsset, typename TLoader>
		static void InitialiseMethod(eng::Asset& asset, const eng::AssetLoader& loader);
		/// \brief Function that can be used to shutdown an asset before it is unloaded.
		template<typename TAsset, typename TLoader>
		static void ShutdownMethod(eng::Asset& asset, const eng::AssetLoader& loader);
		/// \brief Function that can be used to save the asset to a visitor.
		template<typename TAsset, typename TLoader>
		static bool SaveMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);
		/// \brief Function that can be used to load the asset from a visitor.
		template<typename TAsset, typename TLoader>
		static bool LoadMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);
		/// \brief Function that can be used to import an asset from a file.
		template<typename TAsset, typename TLoader>
		static bool ImportMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor);

	private:
		std::mutex m_Mutex;
		Array<eng::Asset*> m_Loaded = {};

		FileMap m_FileMap = { };
		RefMap m_RefMap = { };
		Registry m_Registry = { };
		TypeMap m_TypeMap = { };
	};
}

#include "AssetManager.inl"