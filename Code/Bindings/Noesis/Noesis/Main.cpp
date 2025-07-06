https://www.noesisengine.com/docs/Gui.Core.SDKGuide.html

#include "NoesisPCH.h"

#include "Core/GameTime.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/Uri.h>
#include <NsGui/UserControl.h>
#include <NsRender/GLFactory.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

int NsMain(int argc, char** argv)
{
	NS_UNUSED(argc, argv);

	eng::WindowManager manager;
	manager.Initialise();
	eng::Window& window = *manager.GetWindow(0);
	Vector2u resolution = window.GetResolution();

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

	constexpr const char* root = ".";
	Noesis::GUI::SetFontProvider(Noesis::MakePtr<NoesisApp::LocalFontProvider>(root));
	Noesis::GUI::SetTextureProvider(Noesis::MakePtr<NoesisApp::LocalTextureProvider>(root));
	Noesis::GUI::SetXamlProvider(Noesis::MakePtr<NoesisApp::LocalXamlProvider>(root));

	const char* fonts[] = { "Fonts/#PT Root UI", "Arial", "Segoe UI Emoji" };
	Noesis::GUI::SetFontFallbacks(fonts, 3);
	Noesis::GUI::SetFontDefaultProperties(15.0f, Noesis::FontWeight_Normal, Noesis::FontStretch_Normal, Noesis::FontStyle_Normal);

	NoesisApp::SetThemeProviders();

	Noesis::GUI::LoadApplicationResources(NoesisApp::Theme::DarkBlue());

	Noesis::Ptr<Noesis::FrameworkElement> xaml = Noesis::GUI::LoadXaml<Noesis::UserControl>("Settings.xaml");
	Noesis::Ptr<Noesis::IView> view = Noesis::GUI::CreateView(xaml);
	view->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
	view->SetSize(resolution.x, resolution.y);

	Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::GLFactory::CreateDevice(false);
	view->GetRenderer()->Init(device);

	GameTime gameTime;
	double currTime = 0.0;
	double lastTime = 0.0;
	while (!window.ShouldClose())
	{
		lastTime = currTime;
		currTime = glfwGetTime();

		gameTime.m_DeltaTime = static_cast<float>(currTime - lastTime);
		gameTime.m_TotalTime += gameTime.m_DeltaTime;
		gameTime.m_Frame++;

		window.PreUpdate(gameTime);
		resolution = window.GetResolution();

		//view->MouseButtonDown();
		//view->MouseButtonUp();
		//view->MouseDoubleClick();
		//view->MouseMove();
		//view->MouseWheel();
		//view->MouseHWheel();
		//view->KeyDown();
		//view->KeyUp();

		view->SetSize(resolution.x, resolution.y);
		view->Update(currTime - lastTime);

		{
			// The offscreen rendering phase must be done before binding the framebuffer. This step
			// populates all the internal textures that are needed for the active frame
			view->GetRenderer()->UpdateRenderTree();
			view->GetRenderer()->RenderOffscreen();

			// Bind framebuffer and viewport. Do this per frame because the offscreen phase alters them
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, resolution.x, resolution.y);
			glDisable(GL_SCISSOR_TEST);

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
			glClearStencil(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// -> # At this point, you can render your 3D scene... # <-
			// Note that each function invocation of the renderer modifies the GPU state, you must
			// save and restore it properly in your application

			// After rendering the scene we draw the UI on top of it. Note that the render is performed
			// in the active framebuffer, being it the backbuffer or a render texture
			view->GetRenderer()->Render();
		}

		window.PostUpdate(gameTime);
	}

	view->GetRenderer()->Shutdown();
	view.Reset();
	xaml.Reset();

	Noesis::GUI::Shutdown();

	manager.Shutdown();

	return 0;
}

int main(int argc, char** argv)
{
	return NsMain(argc, argv);
}
