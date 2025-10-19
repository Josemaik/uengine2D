#pragma once

struct InputManager
{
	//input related functions - keys
	bool isKeyPressed(int key) const { return m_keyPressedStates[key]; }
	bool isKeyReleased(int key) const { return m_keyReleaseStates[key]; }
	bool isKeyDown(int key) const {
		return m_keyStates[key] == GLFW_PRESS || m_keyStates[key] == GLFW_REPEAT;
	}
	// Callback de teclado
	static void keyCallback(GLFWwindow* window, int key, int, int action, int) {
		auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		if (key >= 0) {
			im->m_keyStates[key] = action;

			if (action == GLFW_RELEASE)
				im->m_keyReleaseStates[key] = true;
			else if (action == GLFW_PRESS)
				im->m_keyPressedStates[key] = true;
		}
	}
	//input related functions - mouse
	bool isMouseButtonDown(int button) const { return m_mouseButtonStates[button] == GLFW_PRESS; }
	bool isMouseButtonPressed(int button) const { return m_mouseButtonPressedStates[button]; }
	//Callback de mmouse
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int) {
		auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

		if (button >= 0) {
			im->m_mouseButtonStates[button] = action;

			if (action == GLFW_RELEASE)
				im->m_mouseButtonReleaseStates[button] = true;
			else if (action == GLFW_PRESS)
				im->m_mouseButtonPressedStates[button] = true;
		}
	}
	//Is any key down
	bool isAnyKeyDown() {
		for (int i = 0; i < GLFW_KEY_LAST; i++) {
			if (m_keyStates[i] == GLFW_PRESS || m_keyStates[i] == GLFW_REPEAT)
				return true;
		}
		return false;
	}
	static InputManager& getInstance() {
		static InputManager instance;
		return instance;
	}

	void updateEndFrame() {
		// Update key state
		for (auto& sta : m_keyReleaseStates) sta = false;
		for (auto& sta : m_keyPressedStates) sta = false;
		// Update mouse state
		for (auto& sta : m_mouseButtonReleaseStates) sta = false;
		for (auto& sta : m_mouseButtonPressedStates) sta = false;
	}
private:
	// Key buttom states
	int m_keyStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
	int m_keyReleaseStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
	int m_keyPressedStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
	// Mouse Button States
	int m_mouseButtonStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
	int m_mouseButtonReleaseStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
	int m_mouseButtonPressedStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
};