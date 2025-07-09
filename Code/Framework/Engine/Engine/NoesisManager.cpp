#include "EnginePCH.h"
#include "Engine/NoesisManager.h"

#include "Core/GameTime.h"
#include "Core/Guid.h"
#include "Core/Path.h"
#include "Engine/Window.h"
#include "Math/Vector.h"

#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsGui/InputEnums.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/Uri.h>
#include <NsGui/UserControl.h>
#include <NsRender/GLFactory.h>

// https://www.noesisengine.com/docs/4.0/Gui.Studio.Documentation_Index.html
// https://www.noesisengine.com/docs/Gui.Core.SDKGuide.html

namespace
{
	Noesis::Key ToKey(const input::EKey value)
	{
		switch (value)
		{
		case input::EKey::A: return Noesis::Key_A;
		}
		return Noesis::Key::Key_None;
	}

	Noesis::MouseButton ToMouse(const input::EKey value)
	{
		switch (value)
		{
		case input::EKey::Mouse_1: return Noesis::MouseButton::MouseButton_Left;
		case input::EKey::Mouse_2: return Noesis::MouseButton::MouseButton_Right;
		case input::EKey::Mouse_3: return Noesis::MouseButton::MouseButton_Middle;
		}
		return Noesis::MouseButton::MouseButton_Count;
	}
}

void ui::NoesisManager::Initialise(const eng::Window& window)
{
	m_Window = &window;
	const Vector2u resolution = m_Window->GetResolution();

	// A logging handler is installed here. You can also install a custom error handler and memory
	// allocator (see IntegrationAPI.h). By default errors are redirected to the logging handler
	Noesis::GUI::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
		{
			// [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
			const char* prefixes[] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR" };
			printf("[NOESIS/%s] %s\n", prefixes[level], msg);
		});

	Noesis::GUI::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);
	Noesis::GUI::Init();

	const str::Path fontPath = str::Path(str::EPath::Assets, "Fonts");
	const str::Path texturePath = str::Path(str::EPath::Assets, "Textures");
	const str::Path xamlPath = str::Path(str::EPath::Assets, "UI");
	Noesis::GUI::SetFontProvider(Noesis::MakePtr<NoesisApp::LocalFontProvider>(fontPath.ToChar()));
	Noesis::GUI::SetTextureProvider(Noesis::MakePtr<NoesisApp::LocalTextureProvider>(texturePath.ToChar()));
	Noesis::GUI::SetXamlProvider(Noesis::MakePtr<NoesisApp::LocalXamlProvider>(xamlPath.ToChar()));

	const char* fonts[] = { "Arial" };
	Noesis::GUI::SetFontFallbacks(fonts, 1);
	Noesis::GUI::SetFontDefaultProperties(
		15.0f,
		Noesis::FontWeight_Normal,
		Noesis::FontStretch_Normal,
		Noesis::FontStyle_Normal);

	NoesisApp::SetThemeProviders();

	Noesis::GUI::LoadApplicationResources(NoesisApp::Theme::DarkBlue());

	{
		Noesis::Ptr<Noesis::FrameworkElement> xaml = Noesis::GUI::LoadXaml<Noesis::UserControl>("MainMenu.xaml");
		Noesis::Ptr<Noesis::IView> view = Noesis::GUI::CreateView(xaml);
		view->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
		view->SetSize(resolution.x, resolution.y);

		Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::GLFactory::CreateDevice(false);
		view->GetRenderer()->Init(device);

		m_Views.Emplace(str::Guid::Generate(), view);
	}
}

void ui::NoesisManager::Shutdown()
{
	if (!m_Window)
		return;

	for (auto& [guid, view] : m_Views)
	{
		view->GetRenderer()->Shutdown();
		view.Reset();
	}

	Noesis::GUI::Shutdown();
}

void ui::NoesisManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const Vector2u resolution = m_Window->GetResolution();
	for (auto& [guid, view] : m_Views)
	{
		view->SetSize(resolution.x, resolution.y);
		view->Update((double)gameTime.m_TotalTime); // #todo: update gameTime to support double
	}
}

void ui::NoesisManager::RenderBegin()
{
	PROFILE_FUNCTION();

	// The offscreen rendering phase must be done before binding the framebuffer. This step
	// populates all the internal textures that are needed for the active frame
	for (auto& [guid, view] : m_Views)
	{
		view->GetRenderer()->UpdateRenderTree();
		view->GetRenderer()->RenderOffscreen();
	}
}

void ui::NoesisManager::RenderFinish()
{
	PROFILE_FUNCTION();

	for (auto& [guid, view] : m_Views)
		view->GetRenderer()->Render();
}

void ui::NoesisManager::ProcessInput(
	const Vector2f& mousePos, 
	const Vector2f& mouseDelta, 
	const Vector2f& scrollDelta, 
	Set<input::EKey>& inout_Held, 
	Set<input::EKey>& inout_Pressed, 
	Set<input::EKey>& inout_Released)
{
	for (auto& [guid, view] : m_Views)
	{
		view->MouseMove((int)mousePos.x, (int)mousePos.y);
		view->MouseHWheel((int)mousePos.x, (int)mousePos.y, (int)mouseDelta.x);
		view->MouseWheel((int)mousePos.x, (int)mousePos.y, (int)mouseDelta.y);

		for (const input::EKey value : inout_Pressed)
		{
			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton_Count)
			{
				if (view->MouseButtonDown((int)mousePos.x, (int)mousePos.y, mouse))
				{
					inout_Pressed.Remove(value);
					break;
				}
			}
		}

		for (const input::EKey value : inout_Released)
		{
			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton_Count)
			{
				if (view->MouseButtonUp((int)mousePos.x, (int)mousePos.y, mouse))
				{
					inout_Released.Remove(value);
					break;
				}
			}
		}
	}
}
