#include "colors.h"

static HANDLE konsole;
static WORD color = 0;

void Colors::Init()
{
	konsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Colors::setBackgroundColor(enum Colors::color_names name)
{
	color = name;
}

void Colors::setTextColor(enum Colors::color_names name)
{
	color = name;

	SetConsoleTextAttribute(konsole, color);

	return;
}