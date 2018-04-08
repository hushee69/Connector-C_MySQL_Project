#pragma once

#ifndef _COLORS_H
#define _COLORS_H

#include <Windows.h>

namespace Colors
{
	enum color_names
	{
		DARK_BLUE = 1, DARK_GREEN, DARK_CYAN, DARK_RED, DARK_PURPLE, PALE_GREEN, DARK_WHITE, GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_PINK, LIGHT_YELLOW, RESET
	};

	void Init();
	void setBackgroundColor(enum color_names color);
	void setTextColor(enum color_names color);
}

#endif
