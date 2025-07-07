#include "EnginePCH.h"
#include "Engine/NoesisManager.h"

#include "Core/GameTime.h"
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

	Noesis::Ptr<Noesis::FrameworkElement> xaml = Noesis::GUI::LoadXaml<Noesis::UserControl>("MainMenu.xaml");
	m_View = Noesis::GUI::CreateView(xaml);
	m_View->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
	m_View->SetSize(resolution.x, resolution.y);

	Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::GLFactory::CreateDevice(false);
	m_View->GetRenderer()->Init(device);
}

void ui::NoesisManager::Shutdown()
{
	if (!m_Window)
		return;

	m_View->GetRenderer()->Shutdown();
	m_View.Reset();

	Noesis::GUI::Shutdown();
}

void ui::NoesisManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const Vector2u resolution = m_Window->GetResolution();
	m_View->SetSize(resolution.x, resolution.y);
	m_View->Update((double)gameTime.m_TotalTime); // #todo: update gameTime to support double
}

void ui::NoesisManager::RenderBegin()
{
	PROFILE_FUNCTION();

	// The offscreen rendering phase must be done before binding the framebuffer. This step
	// populates all the internal textures that are needed for the active frame
	m_View->GetRenderer()->UpdateRenderTree();
	m_View->GetRenderer()->RenderOffscreen();
}

void ui::NoesisManager::RenderFinish()
{
	PROFILE_FUNCTION();

	m_View->GetRenderer()->Render();
}