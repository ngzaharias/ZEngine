#pragma once

#include <vector>

using int32 = int32_t;

namespace imgui
{
	/// \brief A utility class that can be used with imgui windows.
	/// 
	/// Each time a new window (of the same type) is opened, an id can be borrowed to identify it by. 
	/// This is useful as it means that when the same id is opened, it uses the same settings.
	class Identifier final
	{
	public:
		int32 Borrow();
		void Release(int32 id);

	private:
		std::vector<bool> m_Ids = {};
	};
}