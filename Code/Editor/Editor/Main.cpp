#include "EditorPCH.h"
#include "Editor/Application.h"

#include "Engine/ExceptionFilter.h"
#include "Engine/FileHelpers.h"

#include <filesystem>
#include <direct.h>
#include <windows.h>

int main(int agrc, char* argv[])
{
	SetUnhandledExceptionFilter(eng::UnhandledExceptionFilter);

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

	const int chdirResult = _chdir(immediateDirectory.ToChar());
	Z_PANIC(chdirResult == 0, "Failed to swap to immediate directory [{}]", immediateDirectory.ToChar());
	if (chdirResult != 0)
		return -1;

	core::LogInitialise();

	Z_LOG(ELog::Debug, "=================Application Start====================");
	Z_LOG(ELog::Debug, "AppData Directory:   {}", appdataDirectory.ToChar());
	Z_LOG(ELog::Debug, "Assets Directory:    {}", assetsDirectory.ToChar());
	Z_LOG(ELog::Debug, "Config Directory:    {}", configDirectory.ToChar());
	Z_LOG(ELog::Debug, "Executable Filepath: {}", executableFilepath.ToChar());
	Z_LOG(ELog::Debug, "Working Directory:   {}", workingDirectory.ToChar());

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