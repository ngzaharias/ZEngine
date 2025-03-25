#pragma once

#include "Core/Variant.h"

namespace input
{
	enum class EKey
	{
		None,

		// Alphabet
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		// Numbers
		Num_0,
		Num_1,
		Num_2,
		Num_3,
		Num_4,
		Num_5,
		Num_6,
		Num_7,
		Num_8,
		Num_9,

		// Function
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,

		// Number Pad
		Numpad_0,
		Numpad_1,
		Numpad_2,
		Numpad_3,
		Numpad_4,
		Numpad_5,
		Numpad_6,
		Numpad_7,
		Numpad_8,
		Numpad_9,
		Numpad_Add,
		Numpad_Decimal,
		Numpad_Divide,
		Numpad_Enter,
		Numpad_Equal,
		Numpad_Multiply,
		Numpad_Num_Lock,
		Numpad_Subtract,

		// Modifiers
		Alt_L,
		Alt_R,
		Control_L,
		Control_R,
		Shift_L,
		Shift_R,

		// Arrows
		Up,
		Down,
		Left,
		Right,

		// Other
		Apostrophe, 	/* ' */
		Backslash,		/* \ */
		Backspace,
		Bracket_L,		/* [ */
		Bracket_R,		/* ] */
		Caps_Lock,
		Comma, 			/* , */
		Delete,
		End,
		Enter,
		Equal, 			/* = */
		Escape,
		Grave,			/* ` */
		Home,
		Hyphen, 		/* - */
		Insert,
		Page_Down,
		Page_Up,
		Pause,
		Period, 		/* . */
		Print_Screen,
		Scroll_Lock,
		Semicolon,		/* ; */
		Slash, 			/* / */
		Space,
		Tab,

		// Mouse
		Mouse_1,
		Mouse_2,
		Mouse_3,
		Mouse_4,
		Mouse_5,
		Mouse_6,
		Mouse_7,
		Mouse_8,

		// Gamepad
		Gamepad_A,
		Gamepad_B,
		Gamepad_X,
		Gamepad_Y,
		Gamepad_Bumper_L,
		Gamepad_Bumper_R,
		Gamepad_Back,
		Gamepad_Start,
		Gamepad_Guide,
		Gamepad_Thumb_L,
		Gamepad_Thumb_R,
		Gamepad_DPad_U,
		Gamepad_DPad_R,
		Gamepad_DPad_D,
		Gamepad_DPad_L,

		// Keys with multiple names
		Minus = Hyphen,
		Quote = Apostrophe,
		Return = Enter,
		Subtract = Hyphen,
		Tilde = Grave,

		Mouse_Left = Mouse_1,
		Mouse_Right = Mouse_2,
		Mouse_Middle = Mouse_3,

		Gamepad_Cross = Gamepad_A,
		Gamepad_Circle = Gamepad_B,
		Gamepad_Square = Gamepad_X,
		Gamepad_Triangle = Gamepad_Y,
	};
}