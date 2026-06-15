#include "EnginePCH.h"
#include "Engine/Window.h"

void eng::Window::ConsumeFiles(Array<str::Path>& out_Files)
{
	out_Files.Append(std::move(m_Files));
	m_Files.RemoveAll();
}