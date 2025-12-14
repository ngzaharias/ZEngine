#pragma once

namespace eng
{
	struct Asset;
}

namespace eng
{
	struct AssetRef
	{
		eng::Asset* m_Asset = nullptr;
		int32 m_Count = 0;
	};
}