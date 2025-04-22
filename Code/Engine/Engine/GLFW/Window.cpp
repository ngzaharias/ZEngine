#include "EnginePCH.h"
#include "Engine/GLFW/Window.h"

#ifdef Z_GLFW
#include "Input/Key.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	input::EKey ToGamepad(int32 key)
	{
		switch (key)
		{
		case GLFW_GAMEPAD_BUTTON_A:				return input::EKey::Gamepad_A;
		case GLFW_GAMEPAD_BUTTON_B:				return input::EKey::Gamepad_B;
		case GLFW_GAMEPAD_BUTTON_X:				return input::EKey::Gamepad_X;
		case GLFW_GAMEPAD_BUTTON_Y:				return input::EKey::Gamepad_Y;
		case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:	return input::EKey::Gamepad_Bumper_L;
		case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:	return input::EKey::Gamepad_Bumper_R;
		case GLFW_GAMEPAD_BUTTON_BACK:			return input::EKey::Gamepad_Back;
		case GLFW_GAMEPAD_BUTTON_START:			return input::EKey::Gamepad_Start;
		case GLFW_GAMEPAD_BUTTON_GUIDE:			return input::EKey::Gamepad_Guide;
		case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:	return input::EKey::Gamepad_Thumb_L;
		case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:	return input::EKey::Gamepad_Thumb_R;
		case GLFW_GAMEPAD_BUTTON_DPAD_UP:		return input::EKey::Gamepad_DPad_U;
		case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:	return input::EKey::Gamepad_DPad_R;
		case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:		return input::EKey::Gamepad_DPad_D;
		case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:		return input::EKey::Gamepad_DPad_L;
		}
		return input::EKey::None;
	}

	input::EKey ToKeyboard(int32 key)
	{
		switch (key)
		{
			// Alphabet
		case GLFW_KEY_A: return input::EKey::A;
		case GLFW_KEY_B: return input::EKey::B;
		case GLFW_KEY_C: return input::EKey::C;
		case GLFW_KEY_D: return input::EKey::D;
		case GLFW_KEY_E: return input::EKey::E;
		case GLFW_KEY_F: return input::EKey::F;
		case GLFW_KEY_G: return input::EKey::G;
		case GLFW_KEY_H: return input::EKey::H;
		case GLFW_KEY_I: return input::EKey::I;
		case GLFW_KEY_J: return input::EKey::J;
		case GLFW_KEY_K: return input::EKey::K;
		case GLFW_KEY_L: return input::EKey::L;
		case GLFW_KEY_M: return input::EKey::M;
		case GLFW_KEY_N: return input::EKey::N;
		case GLFW_KEY_O: return input::EKey::O;
		case GLFW_KEY_P: return input::EKey::P;
		case GLFW_KEY_Q: return input::EKey::Q;
		case GLFW_KEY_R: return input::EKey::R;
		case GLFW_KEY_S: return input::EKey::S;
		case GLFW_KEY_T: return input::EKey::T;
		case GLFW_KEY_U: return input::EKey::U;
		case GLFW_KEY_V: return input::EKey::V;
		case GLFW_KEY_W: return input::EKey::W;
		case GLFW_KEY_X: return input::EKey::X;
		case GLFW_KEY_Y: return input::EKey::Y;
		case GLFW_KEY_Z: return input::EKey::Z;

			// Numbers
		case GLFW_KEY_0: return input::EKey::Num_0;
		case GLFW_KEY_1: return input::EKey::Num_1;
		case GLFW_KEY_2: return input::EKey::Num_2;
		case GLFW_KEY_3: return input::EKey::Num_3;
		case GLFW_KEY_4: return input::EKey::Num_4;
		case GLFW_KEY_5: return input::EKey::Num_5;
		case GLFW_KEY_6: return input::EKey::Num_6;
		case GLFW_KEY_7: return input::EKey::Num_7;
		case GLFW_KEY_8: return input::EKey::Num_8;
		case GLFW_KEY_9: return input::EKey::Num_9;

			// Function
		case GLFW_KEY_F1:	return input::EKey::F1;
		case GLFW_KEY_F2:	return input::EKey::F2;
		case GLFW_KEY_F3:	return input::EKey::F3;
		case GLFW_KEY_F4:	return input::EKey::F4;
		case GLFW_KEY_F5:	return input::EKey::F5;
		case GLFW_KEY_F6:	return input::EKey::F6;
		case GLFW_KEY_F7:	return input::EKey::F7;
		case GLFW_KEY_F8:	return input::EKey::F8;
		case GLFW_KEY_F9:	return input::EKey::F9;
		case GLFW_KEY_F10:	return input::EKey::F10;
		case GLFW_KEY_F11:	return input::EKey::F11;
		case GLFW_KEY_F12:	return input::EKey::F12;
		case GLFW_KEY_F13:	return input::EKey::F13;
		case GLFW_KEY_F14:	return input::EKey::F14;
		case GLFW_KEY_F15:	return input::EKey::F15;
		case GLFW_KEY_F16:	return input::EKey::F16;
		case GLFW_KEY_F17:	return input::EKey::F17;
		case GLFW_KEY_F18:	return input::EKey::F18;
		case GLFW_KEY_F19:	return input::EKey::F19;
		case GLFW_KEY_F20:	return input::EKey::F20;
		case GLFW_KEY_F21:	return input::EKey::F21;
		case GLFW_KEY_F22:	return input::EKey::F22;
		case GLFW_KEY_F23:	return input::EKey::F23;
		case GLFW_KEY_F24:	return input::EKey::F24;
		case GLFW_KEY_F25:	return input::EKey::F25;

			// Number Pad
		case GLFW_KEY_KP_0:			return input::EKey::Numpad_0;
		case GLFW_KEY_KP_1:			return input::EKey::Numpad_1;
		case GLFW_KEY_KP_2:			return input::EKey::Numpad_2;
		case GLFW_KEY_KP_3:			return input::EKey::Numpad_3;
		case GLFW_KEY_KP_4:			return input::EKey::Numpad_4;
		case GLFW_KEY_KP_5:			return input::EKey::Numpad_5;
		case GLFW_KEY_KP_6:			return input::EKey::Numpad_6;
		case GLFW_KEY_KP_7:			return input::EKey::Numpad_7;
		case GLFW_KEY_KP_8:			return input::EKey::Numpad_8;
		case GLFW_KEY_KP_9:			return input::EKey::Numpad_9;
		case GLFW_KEY_KP_ADD:		return input::EKey::Numpad_Add;
		case GLFW_KEY_KP_DECIMAL:	return input::EKey::Numpad_Decimal;
		case GLFW_KEY_KP_DIVIDE:	return input::EKey::Numpad_Divide;
		case GLFW_KEY_KP_ENTER:		return input::EKey::Numpad_Enter;
		case GLFW_KEY_KP_EQUAL:		return input::EKey::Numpad_Equal;
		case GLFW_KEY_KP_MULTIPLY:	return input::EKey::Numpad_Multiply;
		case GLFW_KEY_KP_SUBTRACT:	return input::EKey::Numpad_Subtract;

			// Modifiers
		case GLFW_KEY_LEFT_ALT:			return input::EKey::Alt_L;
		case GLFW_KEY_LEFT_CONTROL:		return input::EKey::Control_L;
		case GLFW_KEY_LEFT_SHIFT:		return input::EKey::Shift_L;
		case GLFW_KEY_RIGHT_ALT:		return input::EKey::Alt_R;
		case GLFW_KEY_RIGHT_CONTROL:	return input::EKey::Control_R;
		case GLFW_KEY_RIGHT_SHIFT:		return input::EKey::Shift_R;

			// Arrow
		case GLFW_KEY_UP:		return input::EKey::Up;
		case GLFW_KEY_DOWN:		return input::EKey::Down;
		case GLFW_KEY_LEFT:		return input::EKey::Left;
		case GLFW_KEY_RIGHT:	return input::EKey::Right;

			// Others
		case GLFW_KEY_APOSTROPHE:		return input::EKey::Apostrophe;
		case GLFW_KEY_BACKSLASH:		return input::EKey::Backslash;
		case GLFW_KEY_BACKSPACE:		return input::EKey::Backspace;
		case GLFW_KEY_LEFT_BRACKET:		return input::EKey::Bracket_L;
		case GLFW_KEY_RIGHT_BRACKET:	return input::EKey::Bracket_R;
		case GLFW_KEY_CAPS_LOCK:		return input::EKey::Caps_Lock;
		case GLFW_KEY_COMMA:			return input::EKey::Comma;
		case GLFW_KEY_DELETE:			return input::EKey::Delete;
		case GLFW_KEY_END:				return input::EKey::End;
		case GLFW_KEY_ENTER:			return input::EKey::Enter;
		case GLFW_KEY_EQUAL:			return input::EKey::Equal;
		case GLFW_KEY_ESCAPE:			return input::EKey::Escape;
		case GLFW_KEY_GRAVE_ACCENT:		return input::EKey::Grave;
		case GLFW_KEY_HOME:				return input::EKey::Home;
		case GLFW_KEY_INSERT:			return input::EKey::Insert;
		case GLFW_KEY_MINUS:			return input::EKey::Minus;
		case GLFW_KEY_NUM_LOCK:			return input::EKey::Numpad_Num_Lock;
		case GLFW_KEY_PAGE_DOWN:		return input::EKey::Page_Down;
		case GLFW_KEY_PAGE_UP:			return input::EKey::Page_Up;
		case GLFW_KEY_PAUSE:			return input::EKey::Pause;
		case GLFW_KEY_PERIOD:			return input::EKey::Period;
		case GLFW_KEY_PRINT_SCREEN:		return input::EKey::Print_Screen;
		case GLFW_KEY_SCROLL_LOCK:		return input::EKey::Scroll_Lock;
		case GLFW_KEY_SEMICOLON:		return input::EKey::Semicolon;
		case GLFW_KEY_SLASH:			return input::EKey::Slash;
		case GLFW_KEY_SPACE:			return input::EKey::Space;
		case GLFW_KEY_TAB:				return input::EKey::Tab;
		}
		return input::EKey::None;
	}

	input::EKey ToMouse(int32 key)
	{
		switch (key)
		{
		case GLFW_MOUSE_BUTTON_1: return input::EKey::Mouse_1;
		case GLFW_MOUSE_BUTTON_2: return input::EKey::Mouse_2;
		case GLFW_MOUSE_BUTTON_3: return input::EKey::Mouse_3;
		case GLFW_MOUSE_BUTTON_4: return input::EKey::Mouse_4;
		case GLFW_MOUSE_BUTTON_5: return input::EKey::Mouse_5;
		case GLFW_MOUSE_BUTTON_6: return input::EKey::Mouse_6;
		case GLFW_MOUSE_BUTTON_7: return input::EKey::Mouse_7;
		case GLFW_MOUSE_BUTTON_8: return input::EKey::Mouse_8;
		}
		return input::EKey::None;
	}
}

// https://www.glfw.org/docs/3.3/window_guide.html
glfw::Window::Window(const eng::WindowConfig& config)
	: eng::Window(config)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_AUTO_ICONIFY, true);		// full-screen
	glfwWindowHint(GLFW_CENTER_CURSOR, true);		// full-screen
	glfwWindowHint(GLFW_DECORATED, true);			// windowed
	glfwWindowHint(GLFW_FLOATING, false);			// windowed
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, true);
	glfwWindowHint(GLFW_FOCUSED, true);				// windowed
	glfwWindowHint(GLFW_MAXIMIZED, false);			// windowed
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_VISIBLE, true);				// windowed
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);	// windowed

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	m_Window = glfwCreateWindow(
		mode->width,
		mode->height,
		config.m_Name.c_str(), 
		nullptr,
		nullptr);

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, Callback_FramebufferResized);
	glfwSetScrollCallback(m_Window, Callback_ScrollChanged);
}

glfw::Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void glfw::Window::PreUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	glfwPollEvents();

	double posX, posY;
	glfwGetCursorPos(m_Window, &posX, &posY);

	Vector2f mousePos;
	mousePos.x = static_cast<float>(posX);
	mousePos.y = static_cast<float>(posY);
	m_MouseDelta = m_MousePos - mousePos;
	m_MousePos = mousePos;
}

void glfw::Window::PostUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	glfwSwapBuffers(m_Window);
	m_ScrollDelta = Vector2f::Zero;
}

bool glfw::Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void glfw::Window::GatherGamepad(Set<input::EKey>& out_Keys) const
{
	GLFWgamepadstate state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
	{
		for (int32 i = GLFW_GAMEPAD_BUTTON_A; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i)
		{
			if (state.buttons[i] == GLFW_PRESS)
				out_Keys.Add(ToGamepad(i));
		}
	}
}

void glfw::Window::GatherKeyboard(Set<input::EKey>& out_Keys) const
{
	for (int32 i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
	{
		if (glfwGetKey(m_Window, i) == GLFW_PRESS)
			out_Keys.Add(ToKeyboard(i));
	}
}

void glfw::Window::GatherMouse(Set<input::EKey>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const
{
	for (int32 i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_8; ++i)
	{
		if (glfwGetMouseButton(m_Window, i) == GLFW_PRESS)
			out_Keys.Add(ToMouse(i));
	}

	out_Delta = m_MouseDelta;
	out_Position = m_MousePos;
}

void glfw::Window::GatherScroll(Vector2f& out_Delta) const
{
	out_Delta = m_ScrollDelta;
}

void glfw::Window::Refresh()
{
	switch (m_Config.m_Mode)
	{
	case eng::EWindowMode::Borderless:
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// #bug: setting the monitor with a resolution that doesn't match causes the window to become unresponsive
		glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, m_Config.m_RefreshRate);
	} break;

	case eng::EWindowMode::Windowed:
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		const Vector2u& resolution = m_Config.m_Resolution;
		const Vector2i position = Vector2i(
			mode->width / 2 - resolution.x / 2,
			mode->height / 2 - resolution.y / 2);

		glfwSetWindowMonitor(m_Window, nullptr, position.x, position.y, resolution.x, resolution.y, mode->refreshRate);
	} break;
	}
}

void glfw::Window::Callback_FramebufferResized(GLFWwindow* glfwWindow, int width, int height)
{
	auto* window = reinterpret_cast<glfw::Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->m_Config.m_Resolution.x = static_cast<uint32>(width);
	window->m_Config.m_Resolution.y = static_cast<uint32>(height);

	glViewport(0, 0, width, height);
	//glScissor(0, 0, width, height);
}

void glfw::Window::Callback_ScrollChanged(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
	auto* window = reinterpret_cast<glfw::Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->m_ScrollDelta.x = static_cast<float>(xOffset);
	window->m_ScrollDelta.y = static_cast<float>(yOffset);
}

#endif