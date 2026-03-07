#include "EnginePCH.h"
#include "Engine/ExecuteShell.h"

#include <windows.h>

namespace
{
	INT SW_VALUE(const eng::EShellMode mode)
	{
		switch (mode)
		{
		case eng::EShellMode::DEFAULT:
			return SW_SHOWDEFAULT;
		case eng::EShellMode::NORMAL:
			return SW_SHOWNORMAL;
		}

		return SW_SHOWDEFAULT;
	}
}

void eng::ExecuteShell(const char* operation, const char* filepath, const eng::EShellMode mode)
{
#ifndef Z_RELEASE
	// #release: programs that run shell commands will be deleted by windows defender
	const INT nShowCmd = SW_VALUE(mode);
	ShellExecuteA(NULL, operation, filepath, NULL, NULL, nShowCmd);
#endif
}