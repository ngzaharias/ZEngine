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
		using Bind = void(eng::Asset&, const eng::AssetLoader&);
		Bind* m_Bind = nullptr;
		using Unbind = void(eng::Asset&, const eng::AssetLoader&);
		Unbind* m_Unbind = nullptr;

		using Import = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Import* m_Import = nullptr;
		using Load = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Load* m_Load = nullptr;
		using Save = bool(eng::Asset&, const eng::AssetLoader&, eng::Visitor&);
		Save* m_Save = nullptr;
		using Unload = bool(eng::Asset&, const eng::AssetLoader&);
		Unload* m_Unload = nullptr;
	};
}
