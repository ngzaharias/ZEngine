#pragma once

#include "Core/Types.h"

namespace ecs
{
	/// \brief 
	template<typename TSingleton>
	struct Singleton { };

	using SingletonId = int32;
	struct SingletonTag {};
}