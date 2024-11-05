#include <Catch2/catch.hpp>

#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"

namespace
{
	const str::Guid strGuid = GUID("2f0111d089594da7a2442b80f21b318f");
	const str::Name strMyAsset = NAME("MyAsset");

	class MyAsset final : public eng::Asset { };
	class MyLoader final : public eng::AssetLoader
	{
	public:
		bool Load(MyAsset& asset, eng::Visitor& visitor) const { return true; }
	};

	struct RAIIHelper
	{
		RAIIHelper()
		{
			const str::Path root = eng::GetCurrentFilepath().GetParent();
			m_AssetPath = root.GetParent();
			m_AssetPath = m_AssetPath.GetParent();
			m_AssetPath = m_AssetPath.GetParent();
			m_AssetPath += "\\Code\\";
			m_AssetPath += root.GetStem();
			m_AssetPath += "\\Assets\\AssetManager_ut";
			SetPath(str::EPath::Assets, m_AssetPath);

			m_Manager.RegisterAsset<MyAsset, MyLoader>(strMyAsset);
			m_Manager.Initialise();
		}

		~RAIIHelper()
		{
			m_Manager.Shutdown();
		}

		str::Path m_AssetPath = {};
		eng::AssetManager m_Manager = {};
	};
}

TEST_CASE("eng::AssetManager. Test.")
{
	RAIIHelper raiihelper;
	eng::AssetManager& assetManager = raiihelper.m_Manager;

	CHECK(!assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.RequestAsset<MyAsset>(strGuid);
	CHECK(assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.ReleaseAsset<MyAsset>(strGuid);
	CHECK(!assetManager.GetAsset<MyAsset>(strGuid));

	CHECK(!assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.RequestAsset<MyAsset>(strGuid);
	CHECK(assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.RequestAsset<MyAsset>(strGuid);
	CHECK(assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.ReleaseAsset<MyAsset>(strGuid);
	CHECK(assetManager.GetAsset<MyAsset>(strGuid));
	assetManager.ReleaseAsset<MyAsset>(strGuid);
	CHECK(!assetManager.GetAsset<MyAsset>(strGuid));
}
