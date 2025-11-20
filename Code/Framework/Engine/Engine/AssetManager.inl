#pragma once

#include "Core/Profiler.h"
#include "Engine/AssetLoader.h"

namespace _private
{
	template<typename TAsset, typename TLoader>
	using HasInitialiseMethod = decltype(std::declval<TLoader>().Initialise(std::declval<TAsset&>()));
	template<typename TAsset, typename TLoader>
	using HasShutdownMethod = decltype(std::declval<TLoader>().Shutdown(std::declval<TAsset&>()));
	template<typename TAsset, typename TLoader>
	using HasSaveMethod = decltype(std::declval<TLoader>().Save(std::declval<TAsset&>(), std::declval<eng::Visitor&>()));
	template<typename TAsset, typename TLoader>
	using HasLoadMethod = decltype(std::declval<TLoader>().Load(std::declval<TAsset&>(), std::declval<eng::Visitor&>()));
	template<typename TAsset, typename TLoader>
	using HasImportMethod = decltype(std::declval<TLoader>().Import(std::declval<TAsset&>(), std::declval<eng::Visitor&>()));
}

template<typename TAsset, typename TLoader, typename... TArgs>
void eng::AssetManager::RegisterAsset(const str::Name& type, TArgs&&... args)
{
	static_assert(std::is_convertible<TAsset*, eng::Asset*>::value, "Asset must inherit from eng::Asset using the [public] keyword!");
	static_assert(std::is_base_of<eng::Asset, TAsset>::value, "Asset isn't a base of eng::Asset!");

	static_assert(std::is_convertible<TLoader*, eng::AssetLoader*>::value, "Loader must inherit from eng::AssetLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::AssetLoader, TLoader>::value, "Loader isn't a base of eng::AssetLoader!");

	Z_PANIC(!enumerate::Contains(m_Registry, type), "Asset has already been registered! Typ' '{}] ", ToTypeName<TAsset>());

	eng::AssetEntry& entry = m_Registry[type];
	entry.m_Loader = new TLoader(std::forward<TArgs>(args)...);
	entry.m_Loader->m_AssetManager = this;
	entry.m_Load = &Load<TAsset>;

	eng::AssetMethods& methods = entry.m_Methods;
	if constexpr (core::IsDetected<_private::HasInitialiseMethod, TAsset, TLoader>::value)
		methods.m_Initialise = &InitialiseMethod<TAsset, TLoader>;
	if constexpr (core::IsDetected<_private::HasShutdownMethod, TAsset, TLoader>::value)
		methods.m_Shutdown = &ShutdownMethod<TAsset, TLoader>;
	if constexpr (core::IsDetected<_private::HasSaveMethod, TAsset, TLoader>::value)
		methods.m_Save = &SaveMethod<TAsset, TLoader>;
	if constexpr (core::IsDetected<_private::HasLoadMethod, TAsset, TLoader>::value)
		methods.m_Load = &LoadMethod<TAsset, TLoader>;
	if constexpr (core::IsDetected<_private::HasImportMethod, TAsset, TLoader>::value)
		methods.m_Import = &ImportMethod<TAsset, TLoader>;
}

template<class TAsset>
const TAsset* eng::AssetManager::FetchAsset(const str::Guid& guid) const
{
	const auto find = m_RefMap.Find(guid);
	if (find != m_RefMap.end())
		return static_cast<const TAsset*>(find->second.m_Asset);
	return nullptr;
}

template<typename TAsset>
void eng::AssetManager::LoadDeferred(const str::Path filepath)
{
	LoadImmediate<TAsset>(filepath);
}

template<typename TAsset>
void eng::AssetManager::LoadImmediate(const str::Path& filepath)
{
	eng::Visitor visitor;
	if (!visitor.LoadFromFile(filepath))
		return;

	TAsset* asset = new TAsset();
	visitor.Read("m_Guid", asset->m_Guid, asset->m_Guid);
	visitor.Read("m_Name", asset->m_Name, asset->m_Name);
	visitor.Read("m_Type", asset->m_Type, asset->m_Type);

	const eng::AssetEntry& entry = m_Registry.Get(asset->m_Type);
	if (entry.m_Methods.m_Load && !entry.m_Methods.m_Load(*asset, *entry.m_Loader, visitor))
	{
		delete asset;
		return;
	}

	if (entry.m_Methods.m_Initialise)
		entry.m_Methods.m_Initialise(*asset, *entry.m_Loader);

	m_Mutex.lock();
	m_Loaded.Append(asset);
	m_Mutex.unlock();
}

template<typename TAsset>
void eng::AssetManager::Load(eng::AssetManager& manager, const str::Path& filepath)
{
	if (std::is_base_of<eng::DeferredLoad, TAsset>::value)
	{
		std::thread thread(&eng::AssetManager::LoadDeferred<TAsset>, &manager, filepath);
		thread.detach();
	}
	else
	{
		manager.LoadImmediate<TAsset>(filepath);
	}
}

template<typename TAsset, typename TLoader>
void eng::AssetManager::InitialiseMethod(eng::Asset& asset, const eng::AssetLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	tLoader.Initialise(tAsset);
}

template<typename TAsset, typename TLoader>
void eng::AssetManager::ShutdownMethod(eng::Asset& asset, const eng::AssetLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	tLoader.Shutdown(tAsset);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::SaveMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Save(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::LoadMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Load(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::ImportMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Import(tAsset, visitor);
}