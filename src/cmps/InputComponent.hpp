#pragma once
#include <glfw3.h>
struct InputComponent
{
	int move_left{ GLFW_KEY_A };
	int move_right{ GLFW_KEY_D };
	int jump{ GLFW_KEY_SPACE };
	int attack{GLFW_MOUSE_BUTTON_LEFT};
	int restart{ GLFW_KEY_ENTER };
};