#pragma once

namespace eng
{
	class AssetLoader;
	class Visitor;
	struct Asset;
}

namespace eng
{
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
}
