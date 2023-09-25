#include "EditorPCH.h"
#include "Editor/Application.h"

#include <Engine/FileHelpers.h>

#include <direct.h>
#include <windows.h>

int main(int agrc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	const str::Path appdataDirectory = eng::GetAppDataDirectory();
	const str::Path assetDirectory = eng::GetAssetDirectory();
	const str::Path executableFilepath = eng::GetExecutableFilepath();
	const str::Path workingDirectory = eng::GetWorkingDirectory();
	const str::Path immediateDirectory = executableFilepath.GetDirectory();

	str::SetPath(str::EPath::AppData, appdataDirectory);
	str::SetPath(str::EPath::Assets, assetDirectory);
	str::SetPath(str::EPath::Executable, executableFilepath);
	str::SetPath(str::EPath::WorkingDir, workingDirectory);

	// #todo: handle result
	const int chdirResult = _chdir(immediateDirectory.ToChar());
	Z_ASSERT_CRASH(chdirResult == 0, "Failed to swap to immediate directory [{}]", immediateDirectory.ToChar());

	core::LogInitialise();

	Z_LOG(ELog::Debug, "=================Session Start====================");
	Z_LOG(ELog::Debug, "AppData Directory: {}", appdataDirectory.ToChar());
	Z_LOG(ELog::Debug, "Asset Directory:   {}", assetDirectory.ToChar());
	Z_LOG(ELog::Debug, "Binary Filepath:   {}", executableFilepath.ToChar());
	Z_LOG(ELog::Debug, "Working Directory: {}", workingDirectory.ToChar());

	editor::Application* application = new editor::Application();
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