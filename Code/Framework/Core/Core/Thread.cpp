#include "Core/Thread.h"

std::thread::id core::MAIN_THREAD_ID;

bool core::IsMainThread()
{
	return std::this_thread::get_id() == core::MAIN_THREAD_ID;
}

void core::SetMainThread()
{
	core::MAIN_THREAD_ID = std::this_thread::get_id();
}
