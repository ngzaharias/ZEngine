#include "EnginePCH.h"
#include "Engine/ExceptionFilter.h"

#include <DbgHelp.h>

// https://github.com/behnamasadi/cpp_tutorials/blob/master/docs/generating_and_debugging_dump_file.md
LONG eng::UnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers)
{
	HANDLE hDumpFile = CreateFile(L"CrashDump.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pExceptionPointers;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		CloseHandle(hDumpFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
