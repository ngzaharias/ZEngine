#pragma 

namespace eng
{
	enum class EShellMode
	{
		DEFAULT,
		NORMAL,
	};

	void ExecuteShell(const char* operation, const char* filepath, const eng::EShellMode mode);
}