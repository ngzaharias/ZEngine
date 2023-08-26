#include "EnginePCH.h"
#include "Engine/GLFW/Window.h"
#include "Engine/Screen.h"

#ifdef Z_GLFW
#include <Core/Input.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	input::EKeyboard ToKeyboard(int32 key)
	{
		switch (key)
		{
			// Alphabet
		case GLFW_KEY_A: return input::EKeyboard::A;
		case GLFW_KEY_B: return input::EKeyboard::B;
		case GLFW_KEY_C: return input::EKeyboard::C;
		case GLFW_KEY_D: return input::EKeyboard::D;
		case GLFW_KEY_E: return input::EKeyboard::E;
		case GLFW_KEY_F: return input::EKeyboard::F;
		case GLFW_KEY_G: return input::EKeyboard::G;
		case GLFW_KEY_H: return input::EKeyboard::H;
		case GLFW_KEY_I: return input::EKeyboard::I;
		case GLFW_KEY_J: return input::EKeyboard::J;
		case GLFW_KEY_K: return input::EKeyboard::K;
		case GLFW_KEY_L: return input::EKeyboard::L;
		case GLFW_KEY_M: return input::EKeyboard::M;
		case GLFW_KEY_N: return input::EKeyboard::N;
		case GLFW_KEY_O: return input::EKeyboard::O;
		case GLFW_KEY_P: return input::EKeyboard::P;
		case GLFW_KEY_Q: return input::EKeyboard::Q;
		case GLFW_KEY_R: return input::EKeyboard::R;
		case GLFW_KEY_S: return input::EKeyboard::S;
		case GLFW_KEY_T: return input::EKeyboard::T;
		case GLFW_KEY_U: return input::EKeyboard::U;
		case GLFW_KEY_V: return input::EKeyboard::V;
		case GLFW_KEY_W: return input::EKeyboard::W;
		case GLFW_KEY_X: return input::EKeyboard::X;
		case GLFW_KEY_Y: return input::EKeyboard::Y;
		case GLFW_KEY_Z: return input::EKeyboard::Z;

			// Numbers
		case GLFW_KEY_0: return input::EKeyboard::Num_0;
		case GLFW_KEY_1: return input::EKeyboard::Num_1;
		case GLFW_KEY_2: return input::EKeyboard::Num_2;
		case GLFW_KEY_3: return input::EKeyboard::Num_3;
		case GLFW_KEY_4: return input::EKeyboard::Num_4;
		case GLFW_KEY_5: return input::EKeyboard::Num_5;
		case GLFW_KEY_6: return input::EKeyboard::Num_6;
		case GLFW_KEY_7: return input::EKeyboard::Num_7;
		case GLFW_KEY_8: return input::EKeyboard::Num_8;
		case GLFW_KEY_9: return input::EKeyboard::Num_9;

			// Function
		case GLFW_KEY_F1:	return input::EKeyboard::F1;
		case GLFW_KEY_F2:	return input::EKeyboard::F2;
		case GLFW_KEY_F3:	return input::EKeyboard::F3;
		case GLFW_KEY_F4:	return input::EKeyboard::F4;
		case GLFW_KEY_F5:	return input::EKeyboard::F5;
		case GLFW_KEY_F6:	return input::EKeyboard::F6;
		case GLFW_KEY_F7:	return input::EKeyboard::F7;
		case GLFW_KEY_F8:	return input::EKeyboard::F8;
		case GLFW_KEY_F9:	return input::EKeyboard::F9;
		case GLFW_KEY_F10:	return input::EKeyboard::F10;
		case GLFW_KEY_F11:	return input::EKeyboard::F11;
		case GLFW_KEY_F12:	return input::EKeyboard::F12;
		case GLFW_KEY_F13:	return input::EKeyboard::F13;
		case GLFW_KEY_F14:	return input::EKeyboard::F14;
		case GLFW_KEY_F15:	return input::EKeyboard::F15;
		case GLFW_KEY_F16:	return input::EKeyboard::F16;
		case GLFW_KEY_F17:	return input::EKeyboard::F17;
		case GLFW_KEY_F18:	return input::EKeyboard::F18;
		case GLFW_KEY_F19:	return input::EKeyboard::F19;
		case GLFW_KEY_F20:	return input::EKeyboard::F20;
		case GLFW_KEY_F21:	return input::EKeyboard::F21;
		case GLFW_KEY_F22:	return input::EKeyboard::F22;
		case GLFW_KEY_F23:	return input::EKeyboard::F23;
		case GLFW_KEY_F24:	return input::EKeyboard::F24;
		case GLFW_KEY_F25:	return input::EKeyboard::F25;

			// Number Pad
		case GLFW_KEY_KP_0:			return input::EKeyboard::Numpad_0;
		case GLFW_KEY_KP_1:			return input::EKeyboard::Numpad_1;
		case GLFW_KEY_KP_2:			return input::EKeyboard::Numpad_2;
		case GLFW_KEY_KP_3:			return input::EKeyboard::Numpad_3;
		case GLFW_KEY_KP_4:			return input::EKeyboard::Numpad_4;
		case GLFW_KEY_KP_5:			return input::EKeyboard::Numpad_5;
		case GLFW_KEY_KP_6:			return input::EKeyboard::Numpad_6;
		case GLFW_KEY_KP_7:			return input::EKeyboard::Numpad_7;
		case GLFW_KEY_KP_8:			return input::EKeyboard::Numpad_8;
		case GLFW_KEY_KP_9:			return input::EKeyboard::Numpad_9;
		case GLFW_KEY_KP_ADD:		return input::EKeyboard::Numpad_Add;
		case GLFW_KEY_KP_DECIMAL:	return input::EKeyboard::Numpad_Decimal;
		case GLFW_KEY_KP_DIVIDE:	return input::EKeyboard::Numpad_Divide;
		case GLFW_KEY_KP_ENTER:		return input::EKeyboard::Numpad_Enter;
		case GLFW_KEY_KP_EQUAL:		return input::EKeyboard::Numpad_Equal;
		case GLFW_KEY_KP_MULTIPLY:	return input::EKeyboard::Numpad_Multiply;
		case GLFW_KEY_KP_SUBTRACT:	return input::EKeyboard::Numpad_Subtract;

			// Modifiers
		case GLFW_KEY_LEFT_ALT:			return input::EKeyboard::Alt_L;
		case GLFW_KEY_LEFT_CONTROL:		return input::EKeyboard::Control_L;
		case GLFW_KEY_LEFT_SHIFT:		return input::EKeyboard::Shift_L;
		case GLFW_KEY_RIGHT_ALT:		return input::EKeyboard::Alt_R;
		case GLFW_KEY_RIGHT_CONTROL:	return input::EKeyboard::Control_R;
		case GLFW_KEY_RIGHT_SHIFT:		return input::EKeyboard::Shift_R;

			// Arrow
		case GLFW_KEY_UP:		return input::EKeyboard::Up;
		case GLFW_KEY_DOWN:		return input::EKeyboard::Down;
		case GLFW_KEY_LEFT:		return input::EKeyboard::Left;
		case GLFW_KEY_RIGHT:	return input::EKeyboard::Right;

			// Others
		case GLFW_KEY_APOSTROPHE:		return input::EKeyboard::Apostrophe;
		case GLFW_KEY_BACKSLASH:		return input::EKeyboard::Backslash;
		case GLFW_KEY_BACKSPACE:		return input::EKeyboard::Backspace;
		case GLFW_KEY_LEFT_BRACKET:		return input::EKeyboard::Bracket_L;
		case GLFW_KEY_RIGHT_BRACKET:	return input::EKeyboard::Bracket_R;
		case GLFW_KEY_CAPS_LOCK:		return input::EKeyboard::Caps_Lock;
		case GLFW_KEY_COMMA:			return input::EKeyboard::Comma;
		case GLFW_KEY_DELETE:			return input::EKeyboard::Delete;
		case GLFW_KEY_END:				return input::EKeyboard::End;
		case GLFW_KEY_ENTER:			return input::EKeyboard::Enter;
		case GLFW_KEY_EQUAL:			return input::EKeyboard::Equal;
		case GLFW_KEY_ESCAPE:			return input::EKeyboard::Escape;
		case GLFW_KEY_GRAVE_ACCENT:		return input::EKeyboard::Grave;
		case GLFW_KEY_HOME:				return input::EKeyboard::Home;
		case GLFW_KEY_INSERT:			return input::EKeyboard::Insert;
		case GLFW_KEY_MINUS:			return input::EKeyboard::Minus;
		case GLFW_KEY_NUM_LOCK:			return input::EKeyboard::Numpad_Num_Lock;
		case GLFW_KEY_PAGE_DOWN:		return input::EKeyboard::Page_Down;
		case GLFW_KEY_PAGE_UP:			return input::EKeyboard::Page_Up;
		case GLFW_KEY_PAUSE:			return input::EKeyboard::Pause;
		case GLFW_KEY_PERIOD:			return input::EKeyboard::Period;
		case GLFW_KEY_PRINT_SCREEN:		return input::EKeyboard::Print_Screen;
		case GLFW_KEY_SCROLL_LOCK:		return input::EKeyboard::Scroll_Lock;
		case GLFW_KEY_SEMICOLON:		return input::EKeyboard::Semicolon;
		case GLFW_KEY_SLASH:			return input::EKeyboard::Slash;
		case GLFW_KEY_SPACE:			return input::EKeyboard::Space;
		case GLFW_KEY_TAB:				return input::EKeyboard::Tab;
		}
		return input::EKeyboard::Unknown;
	}

	input::EMouse ToMouse(int32 key)
	{
		switch (key)
		{
		case GLFW_MOUSE_BUTTON_1: return input::EMouse::Button_1;
		case GLFW_MOUSE_BUTTON_2: return input::EMouse::Button_2;
		case GLFW_MOUSE_BUTTON_3: return input::EMouse::Button_3;

		case GLFW_MOUSE_BUTTON_4: return input::EMouse::Button_4;
		case GLFW_MOUSE_BUTTON_5: return input::EMouse::Button_5;
		case GLFW_MOUSE_BUTTON_6: return input::EMouse::Button_6;
		case GLFW_MOUSE_BUTTON_7: return input::EMouse::Button_7;
		case GLFW_MOUSE_BUTTON_8: return input::EMouse::Button_8;
		}
		return input::EMouse::Unknown;
	}

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}
}

glfw::Window::Window(const eng::WindowConfig& config)
	: eng::IWindow(config)
{
	constexpr bool s_IsFullscreen = false;

	m_Name = config.m_Name;
	m_Size = config.m_Size;

	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* fullscreen = nullptr;
	if (s_IsFullscreen)
		fullscreen = glfwGetPrimaryMonitor();

	m_Window = glfwCreateWindow(config.m_Size.x, config.m_Size.y, m_Name.c_str(), fullscreen, nullptr);

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);

	// V-Sync
	glfwSwapInterval(0);

	auto error = glewInit();
	if (error != GLEW_OK)
	{
		auto message = glewGetErrorString(error);
		printf("Error: %s\n", message);
	}
}

glfw::Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void glfw::Window::Initialize()
{
}

void glfw::Window::Shutdown()
{
}

void glfw::Window::PreUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	{
		constexpr Vector4f s_ClearColour = Vector4f(0.117f, 0.117f, 0.117f, 1.f);

		glClearDepthf(1.f);
		glClearColor(s_ClearColour.x, s_ClearColour.y, s_ClearColour.z, s_ClearColour.w);

		// the depth mask must be enabled BEFORE clearing the depth buffer
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

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
}

bool glfw::Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void glfw::Window::GatherKeyboard(Set<input::EKeyboard>& out_Keys) const
{
	for (int32 i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; ++i)
	{
		if (glfwGetKey(m_Window, i) == GLFW_PRESS)
			out_Keys.Add(ToKeyboard(i));
	}
}

void glfw::Window::GatherMouse(Set<input::EMouse>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const
{
	for (int32 i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_8; ++i)
	{
		if (glfwGetMouseButton(m_Window, i) == GLFW_PRESS)
			out_Keys.Add(ToMouse(i));
	}

	out_Delta = m_MouseDelta;
	out_Position = m_MousePos;
}

void glfw::Window::FramebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	auto* window = reinterpret_cast<glfw::Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->m_HasResized = true;
	window->m_Size.x = static_cast<uint32>(width);
	window->m_Size.y = static_cast<uint32>(height);
	Screen::width = static_cast<float>(window->m_Size.x);
	Screen::height = static_cast<float>(window->m_Size.y);

	glViewport(0, 0, width, height);
	//glScissor(0, 0, width, height);
}

#endif