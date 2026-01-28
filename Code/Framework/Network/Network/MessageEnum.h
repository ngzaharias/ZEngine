#pragma once

namespace net
{
	enum EMessage : uint32
	{
		Unassigned = 0,

		ECS_First,
		ECS_Last = ECS_First + 32,
	};
}