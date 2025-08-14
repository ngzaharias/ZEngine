#include "EnginePCH.h"
#include "Engine/UIManager.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/Set.h"
#include "Engine/Window.h"
#include "Input/Key.h"
#include "Math/Vector.h"

#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>
#include <NsCore/RegisterComponent.h>
#include <NsGui/InputEnums.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/Uri.h>
#include <NsGui/UserControl.h>
#include <NsRender/GLFactory.h>
#include <NoesisGUI/License.h>

// https://www.noesisengine.com/docs/4.0/Gui.Studio.Documentation_Index.html
// https://www.noesisengine.com/docs/Gui.Core.SDKGuide.html

namespace
{
	const str::Name strMainMenu = NAME("MainMenu.xaml");

	Noesis::Key ToKey(const input::EKey value)
	{
		switch (value)
		{
		case input::EKey::A: return Noesis::Key_A;
		case input::EKey::B: return Noesis::Key_B;
		case input::EKey::C: return Noesis::Key_C;
		case input::EKey::D: return Noesis::Key_D;
		case input::EKey::E: return Noesis::Key_E;
		case input::EKey::F: return Noesis::Key_F;
		case input::EKey::G: return Noesis::Key_G;
		case input::EKey::H: return Noesis::Key_H;
		case input::EKey::I: return Noesis::Key_I;
		case input::EKey::J: return Noesis::Key_J;
		case input::EKey::K: return Noesis::Key_K;
		case input::EKey::L: return Noesis::Key_L;
		case input::EKey::M: return Noesis::Key_M;
		case input::EKey::N: return Noesis::Key_N;
		case input::EKey::O: return Noesis::Key_O;
		case input::EKey::P: return Noesis::Key_P;
		case input::EKey::Q: return Noesis::Key_Q;
		case input::EKey::R: return Noesis::Key_R;
		case input::EKey::S: return Noesis::Key_S;
		case input::EKey::T: return Noesis::Key_T;
		case input::EKey::U: return Noesis::Key_U;
		case input::EKey::V: return Noesis::Key_V;
		case input::EKey::W: return Noesis::Key_W;
		case input::EKey::X: return Noesis::Key_X;
		case input::EKey::Y: return Noesis::Key_Y;
		case input::EKey::Z: return Noesis::Key_Z;

		case input::EKey::Num_0: return Noesis::Key_NumPad0;
		case input::EKey::Num_1: return Noesis::Key_NumPad1;
		case input::EKey::Num_2: return Noesis::Key_NumPad2;
		case input::EKey::Num_3: return Noesis::Key_NumPad3;
		case input::EKey::Num_4: return Noesis::Key_NumPad4;
		case input::EKey::Num_5: return Noesis::Key_NumPad5;
		case input::EKey::Num_6: return Noesis::Key_NumPad6;
		case input::EKey::Num_7: return Noesis::Key_NumPad7;
		case input::EKey::Num_8: return Noesis::Key_NumPad8;
		case input::EKey::Num_9: return Noesis::Key_NumPad9;

		case input::EKey::F1:	return Noesis::Key_F1;
		case input::EKey::F2:	return Noesis::Key_F2;
		case input::EKey::F3:	return Noesis::Key_F3;
		case input::EKey::F4:	return Noesis::Key_F4;
		case input::EKey::F5:	return Noesis::Key_F5;
		case input::EKey::F6:	return Noesis::Key_F6;
		case input::EKey::F7:	return Noesis::Key_F7;
		case input::EKey::F8:	return Noesis::Key_F8;
		case input::EKey::F9:	return Noesis::Key_F9;
		case input::EKey::F10:	return Noesis::Key_F10;
		case input::EKey::F11:	return Noesis::Key_F11;
		case input::EKey::F12:	return Noesis::Key_F12;
		case input::EKey::F13:	return Noesis::Key_F13;
		case input::EKey::F14:	return Noesis::Key_F14;
		case input::EKey::F15:	return Noesis::Key_F15;
		case input::EKey::F16:	return Noesis::Key_F16;
		case input::EKey::F17:	return Noesis::Key_F17;
		case input::EKey::F18:	return Noesis::Key_F18;
		case input::EKey::F19:	return Noesis::Key_F19;
		case input::EKey::F20:	return Noesis::Key_F20;
		case input::EKey::F21:	return Noesis::Key_F21;
		case input::EKey::F22:	return Noesis::Key_F22;
		case input::EKey::F23:	return Noesis::Key_F23;
		case input::EKey::F24:	return Noesis::Key_F24;

		case input::EKey::Numpad_0:			return Noesis::Key_NumPad0;
		case input::EKey::Numpad_1:			return Noesis::Key_NumPad1;
		case input::EKey::Numpad_2:			return Noesis::Key_NumPad2;
		case input::EKey::Numpad_3:			return Noesis::Key_NumPad3;
		case input::EKey::Numpad_4:			return Noesis::Key_NumPad4;
		case input::EKey::Numpad_5:			return Noesis::Key_NumPad5;
		case input::EKey::Numpad_6:			return Noesis::Key_NumPad6;
		case input::EKey::Numpad_7:			return Noesis::Key_NumPad7;
		case input::EKey::Numpad_8:			return Noesis::Key_NumPad8;
		case input::EKey::Numpad_9:			return Noesis::Key_NumPad9;
		case input::EKey::Numpad_Add:		return Noesis::Key_Add;
		case input::EKey::Numpad_Decimal:	return Noesis::Key_Decimal;
		case input::EKey::Numpad_Divide:	return Noesis::Key_Divide;
		case input::EKey::Numpad_Enter:		return Noesis::Key_Enter;
		case input::EKey::Numpad_Multiply:	return Noesis::Key_Multiply;
		case input::EKey::Numpad_Subtract:	return Noesis::Key_Subtract;
		case input::EKey::Numpad_Num_Lock:	return Noesis::Key_NumLock;

		case input::EKey::Shift_L:		return Noesis::Key_LeftShift;
		case input::EKey::Shift_R:		return Noesis::Key_RightShift;
		case input::EKey::Control_L:	return Noesis::Key_LeftCtrl;
		case input::EKey::Control_R:	return Noesis::Key_RightCtrl;
		case input::EKey::Alt_L:		return Noesis::Key_LeftAlt;
		case input::EKey::Alt_R:		return Noesis::Key_RightAlt;

		case input::EKey::Up:		return Noesis::Key_Up;
		case input::EKey::Down:		return Noesis::Key_Down;
		case input::EKey::Left:		return Noesis::Key_Left;
		case input::EKey::Right:	return Noesis::Key_Right;

		case input::EKey::Apostrophe:	return Noesis::Key_Oem7;
		case input::EKey::Backslash:	return Noesis::Key_Oem5;
		case input::EKey::Backspace:	return Noesis::Key_Back;
		case input::EKey::Bracket_L:	return Noesis::Key_OemOpenBrackets;
		case input::EKey::Bracket_R:	return Noesis::Key_OemCloseBrackets;
		case input::EKey::Caps_Lock:	return Noesis::Key_CapsLock;
		case input::EKey::Comma:		return Noesis::Key_OemComma;
		case input::EKey::Delete:		return Noesis::Key_Delete;
		case input::EKey::End:			return Noesis::Key_End;
		case input::EKey::Enter:		return Noesis::Key_Enter;
		case input::EKey::Equal:		return Noesis::Key_OemPlus;
		case input::EKey::Escape:		return Noesis::Key_Escape;
		case input::EKey::Grave:		return Noesis::Key_Oem3;
		case input::EKey::Home:			return Noesis::Key_Home;
		case input::EKey::Hyphen:		return Noesis::Key_OemMinus;
		case input::EKey::Insert:		return Noesis::Key_Insert;
		case input::EKey::Page_Down:	return Noesis::Key_PageDown;
		case input::EKey::Page_Up:		return Noesis::Key_PageUp;
		case input::EKey::Pause:		return Noesis::Key_Pause;
		case input::EKey::Period:		return Noesis::Key_OemPeriod;
		case input::EKey::Print_Screen:	return Noesis::Key_PrintScreen;
		case input::EKey::Scroll_Lock:	return Noesis::Key_Scroll;
		case input::EKey::Semicolon:	return Noesis::Key_Oem1;
		case input::EKey::Slash:		return Noesis::Key_Oem2;
		case input::EKey::Space:		return Noesis::Key_Space;
		case input::EKey::Tab:			return Noesis::Key_Tab;
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
		case input::EKey::Mouse_4: return Noesis::MouseButton::MouseButton_XButton1;
		case input::EKey::Mouse_5: return Noesis::MouseButton::MouseButton_XButton2;
		}
		return Noesis::MouseButton::MouseButton_Count;
	}
}

eng::UIManager::UIManager(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void eng::UIManager::Initialise(const eng::Window& window)
{
	m_Window = &window;

	// A logging handler is installed here. You can also install a custom error handler and memory
	// allocator (see IntegrationAPI.h). By default errors are redirected to the logging handler
	Noesis::GUI::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
		{
			// [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
			const char* prefixes[] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR" };
			printf("[NOESIS/%s] %s\n", prefixes[level], msg);
		});

	Noesis::GUI::SetLicense(Noesis::strName, Noesis::strKey);
	Noesis::GUI::Init();

	const str::Path fontPath = str::Path(str::EPath::Assets);
	const str::Path texturePath = str::Path(str::EPath::Assets);
	const str::Path xamlPath = str::Path(str::EPath::Assets, "UI");
	Noesis::GUI::SetFontProvider(Noesis::MakePtr<NoesisApp::LocalFontProvider>(fontPath.ToChar()));
	Noesis::GUI::SetTextureProvider(Noesis::MakePtr<NoesisApp::LocalTextureProvider>(texturePath.ToChar()));
	Noesis::GUI::SetXamlProvider(Noesis::MakePtr<NoesisApp::LocalXamlProvider>(xamlPath.ToChar()));

	const char* fonts[] = { "Figtree" };
	Noesis::GUI::SetFontFallbacks(fonts, 1);
	Noesis::GUI::SetFontDefaultProperties(
		15.0f,
		Noesis::FontWeight_Normal,
		Noesis::FontStretch_Normal,
		Noesis::FontStyle_Normal);

	NoesisApp::SetThemeProviders();

	Noesis::GUI::LoadApplicationResources("Themes/Default.xaml");
}

void eng::UIManager::Shutdown()
{
	if (!m_Window)
		return;

	for (auto& [name, widget] : m_Widgets)
		widget.m_View->GetRenderer()->Shutdown();
	m_Widgets.RemoveAll();

	m_DataContexts.RemoveAll();

	Noesis::GUI::Shutdown();
}

void eng::UIManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const Vector2u resolution = m_Window->GetResolution();
	for (const str::Name& name : m_Stack)
	{
		eng::Widget& widget = m_Widgets.Get(name);
		widget.m_View->SetSize(resolution.x, resolution.y);
		widget.m_View->Update((double)gameTime.m_TotalTime); // #todo: update gameTime to support double
	}
}

void eng::UIManager::RenderBegin()
{
	PROFILE_FUNCTION();

	// The offscreen rendering phase must be done before binding the framebuffer. This step
	// populates all the internal textures that are needed for the active frame
	for (const str::Name& name : m_Stack)
	{
		eng::Widget& widget = m_Widgets.Get(name);
		widget.m_View->GetRenderer()->UpdateRenderTree();
		widget.m_View->GetRenderer()->RenderOffscreen();
	}
}

void eng::UIManager::RenderFinish()
{
	PROFILE_FUNCTION();

	for (auto& [name, widget] : m_Widgets)
		widget.m_View->GetRenderer()->Render();
}

void eng::UIManager::ProcessInput(
	const Vector2f& mousePos,
	const Vector2f& mouseDelta,
	const Vector2f& scrollDelta,
	Set<input::EKey>& inout_Held,
	Set<input::EKey>& inout_Pressed,
	Set<input::EKey>& inout_Released)
{
	for (const auto& [i, name] : enumerate::Reverse(m_Stack))
	{
		Set<input::EKey> pressed;
		Set<input::EKey> released;

		eng::Widget& widget = m_Widgets.Get(name);
		Noesis::Ptr<Noesis::IView> view = widget.m_View;
		view->MouseMove((int)mousePos.x, (int)mousePos.y);
		view->MouseHWheel((int)mousePos.x, (int)mousePos.y, (int)scrollDelta.x);
		view->MouseWheel((int)mousePos.x, (int)mousePos.y, (int)scrollDelta.y * 50);

		for (const input::EKey value : inout_Pressed)
		{
			const Noesis::Key key = ToKey(value);
			if (key != Noesis::Key_None)
			{
				if (view->KeyDown(key))
					pressed.Add(value);
			}

			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton_Count)
			{
				if (view->MouseButtonDown((int)mousePos.x, (int)mousePos.y, mouse))
					pressed.Add(value);
			}
		}

		for (const input::EKey value : inout_Released)
		{
			const Noesis::Key key = ToKey(value);
			if (key != Noesis::Key_None)
			{
				if (view->KeyUp(key))
					released.Add(value);
			}

			const Noesis::MouseButton mouse = ToMouse(value);
			if (mouse != Noesis::MouseButton_Count)
			{
				if (view->MouseButtonUp((int)mousePos.x, (int)mousePos.y, mouse))
					released.Add(value);
			}
		}

		inout_Pressed.Remove(pressed);
		inout_Released.Remove(released);
		if (widget.m_CosumeAll)
		{
			inout_Held.RemoveAll();
			inout_Pressed.RemoveAll();
			inout_Released.RemoveAll();
		}
	}
}

void eng::UIManager::CreateWidget(const str::Name& name, const bool consumeAll)
{
	const Vector2u& resolution = m_Window->GetResolution();

	Noesis::Ptr<Noesis::UserControl> xaml = Noesis::GUI::LoadXaml<Noesis::UserControl>(name.ToChar());
	if (m_DataContexts.Contains(name))
	{
		auto dataContext = m_DataContexts[name];
		xaml->SetDataContext(dataContext);
	}

	Noesis::Ptr<Noesis::IView> view = Noesis::GUI::CreateView(xaml);
	view->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
	view->SetSize(resolution.x, resolution.y);

	Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::GLFactory::CreateDevice(false);
	view->GetRenderer()->Init(device);

	m_Widgets[name] = eng::Widget{ view, consumeAll };
	m_Stack.Emplace(name);
}

void eng::UIManager::DestroyWidget(const str::Name& name)
{
	m_Widgets.Remove(name);

	for (const auto& [i, value] : enumerate::Forward(m_Stack))
	{
		if (value == name)
		{
			m_Stack.RemoveOrderedAt(i);
			break;
		}
	}
}
