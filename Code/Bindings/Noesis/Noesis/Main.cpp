https://www.noesisengine.com/docs/Gui.Core.SDKGuide.html

#include "NoesisPCH.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsGui/InputEnums.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/Uri.h>
#include <NsGui/UserControl.h>
#include <NsRender/GLFactory.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

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

int NsMain(int argc, char** argv)
{
	NS_UNUSED(argc, argv);

	eng::WindowManager windowManager;
	windowManager.Initialise();
	eng::Window& window = *windowManager.GetWindow(0);
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

	Vector2f mouseDelta = Vector2f::Zero;
	Vector2f mousePosition = Vector2f::Zero;
	Vector2f scrollDelta = Vector2f::Zero;
	Set<input::EKey> inputCurrent;
	Set<input::EKey> inputPrevious;
	while (!window.ShouldClose())
	{
		lastTime = currTime;
		currTime = glfwGetTime();

		gameTime.m_DeltaTime = static_cast<float>(currTime - lastTime);
		gameTime.m_TotalTime += gameTime.m_DeltaTime;
		gameTime.m_Frame++;

		windowManager.PreUpdate(gameTime);
		resolution = window.GetResolution();
		view->SetSize(resolution.x, resolution.y);


		std::swap(inputPrevious, inputCurrent);
		inputCurrent.RemoveAll();

		window.GatherGamepad(inputCurrent);
		window.GatherKeyboard(inputCurrent);
		window.GatherMouse(inputCurrent, mouseDelta, mousePosition);
		window.GatherScroll(scrollDelta);

		Set<input::EKey> held, pressed, released;
		enumerate::Intersection(inputPrevious, inputCurrent, held);
		enumerate::Difference(inputCurrent, inputPrevious, pressed);
		enumerate::Difference(inputPrevious, inputCurrent, released);

		for (const input::EKey value : pressed)
		{
			const Noesis::Key key = ToKey(value);
			if (key != Noesis::Key::Key_None)
				view->KeyDown(key);

			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton::MouseButton_Count)
				view->MouseButtonDown((int)mousePosition.x, (int)mousePosition.y, mouse);
		}

		for (const input::EKey value : released)
		{
			const Noesis::Key key = ToKey(value);
			if (key != Noesis::Key::Key_None)
				view->KeyUp(key);

			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton::MouseButton_Count)
				view->MouseButtonUp((int)mousePosition.x, (int)mousePosition.y, mouse);
		}

		//view->MouseButtonDown();
		//view->MouseButtonUp();
		//view->MouseDoubleClick();
		view->MouseMove((int)mousePosition.x, (int)mousePosition.y);
		//view->MouseWheel();
		//view->MouseHWheel();

		view->Update(currTime);

		{
			// The offscreen rendering phase must be done before binding the framebuffer. This step
			// populates all the internal textures that are needed for the active frame
			view->GetRenderer()->UpdateRenderTree();
			view->GetRenderer()->RenderOffscreen();

			// Bind framebuffer and viewport. Do this per frame because the offscreen phase alters them
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, resolution.x, resolution.y);
			glDisable(GL_SCISSOR_TEST);
			glClearStencil(0);

			// Always clear the stencil, important in tiled architectures to avoid loading
			glColorMask(true, true, true, true);
			glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
			glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			// -> # At this point, you can render your 3D scene... # <-
			// Note that each function invocation of the renderer modifies the GPU state, you must
			// save and restore it properly in your application

			// After rendering the scene we draw the UI on top of it. Note that the render is performed
			// in the active framebuffer, being it the backbuffer or a render texture
			view->GetRenderer()->Render();
		}

		windowManager.PostUpdate(gameTime);
	}

	view->GetRenderer()->Shutdown();
	view.Reset();
	xaml.Reset();

	Noesis::GUI::Shutdown();

	windowManager.Shutdown();

	return 0;
}

int main(int argc, char** argv)
{
	return NsMain(argc, argv);
}
