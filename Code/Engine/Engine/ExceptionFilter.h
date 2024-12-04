#pragma once

#include <windows.h>

namespace eng
{
	LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers);
}