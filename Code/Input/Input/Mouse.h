#pragma once

namespace input
{
	enum class EMouse
	{
		Unknown,

		// Main
		Button_1,
		Button_2,
		Button_3,

		// Extra
		Button_4,
		Button_5,
		Button_6,
		Button_7,
		Button_8,

		// Keys with multiple names
		Left = Button_1,
		Right = Button_2,
		Middle = Button_3,
	};
};