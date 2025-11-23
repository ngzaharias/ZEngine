#pragma once

#include <thread>

namespace core
{
	extern std::thread::id MAIN_THREAD_ID;

	/// \brief Checks if the thread the function is called from is the thread the application was launched on.
	bool IsMainThread();

	/// \brief Sets the main thread to be the thread the function was called from.
	void SetMainThread();
}