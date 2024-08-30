#include "GamePCH.h"
#include "Game/Application.h"

#include "Engine/FileHelpers.h"

#include <filesystem>
#include <direct.h>
#include <windows.h>
#include <DbgHelp.h>

	// https://github.com/behnamasadi/cpp_tutorials/blob/master/docs/generating_and_debugging_dump_file.md
LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers)
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

int main(int agrc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	const str::Path appdataDirectory = eng::GetAppDataDirectory();
	const str::Path assetsDirectory = eng::GetAssetsDirectory();
	const str::Path configDirectory = eng::GetConfigDirectory();
	const str::Path executableFilepath = eng::GetExecutableFilepath();
	const str::Path workingDirectory = eng::GetWorkingDirectory();
	const str::Path immediateDirectory = executableFilepath.GetDirectory();

	str::SetPath(str::EPath::AppData, appdataDirectory);
	str::SetPath(str::EPath::Assets, assetsDirectory);
	str::SetPath(str::EPath::Config, configDirectory);
	str::SetPath(str::EPath::Executable, executableFilepath);
	str::SetPath(str::EPath::WorkingDir, workingDirectory);

	std::filesystem::create_directories(appdataDirectory.ToChar());

	// #todo: handle result
	const int chdirResult = _chdir(immediateDirectory.ToChar());
	Z_PANIC(chdirResult == 0, "Failed to swap to immediate directory [{}]", immediateDirectory.ToChar());

	core::LogInitialise();

	Z_LOG(ELog::Debug, "=================Session Start====================");
	Z_LOG(ELog::Debug, "AppData Directory:   {}", appdataDirectory.ToChar());
	Z_LOG(ELog::Debug, "Assets Directory:    {}", assetsDirectory.ToChar());
	Z_LOG(ELog::Debug, "Config Directory:    {}", configDirectory.ToChar());
	Z_LOG(ELog::Debug, "Executable Filepath: {}", executableFilepath.ToChar());
	Z_LOG(ELog::Debug, "Working Directory:   {}", workingDirectory.ToChar());

	game::Application* application = new game::Application();
	application->Execute(agrc, argv);
	delete(application);

	core::LogShutdown();
	return 0;
}

#ifdef _WIN32
int WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	return main(__argc, __argv);
}
#endif