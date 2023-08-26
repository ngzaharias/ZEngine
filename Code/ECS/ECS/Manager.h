#pragma once

namespace ecs
{
	class Manager
	{
	public:
		virtual void Initialise() { }
		virtual void Shutdown() { }
	};

	struct ManagerTag {};
}
