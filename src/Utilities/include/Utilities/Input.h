#pragma once
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>

#define PSP_CTRL_ANALOG_X 0x3
#define PSP_CTRL_ANALOG_Y 0x5

#else
#include <GLFW/glfw3.h>

//This is just for intercompat purposes to not error out
enum PspCtrlButtons
{
	/** Select button. */
	PSP_CTRL_SELECT = -1,
	/** Start button. */
	PSP_CTRL_START = -1,
	/** Up D-Pad button. */
	PSP_CTRL_UP = -1,
	/** Right D-Pad button. */
	PSP_CTRL_RIGHT = -1,
	/** Down D-Pad button. */
	PSP_CTRL_DOWN = -1,
	/** Left D-Pad button. */
	PSP_CTRL_LEFT = -1,
	/** Left trigger. */
	PSP_CTRL_LTRIGGER = -1,
	/** Right trigger. */
	PSP_CTRL_RTRIGGER = -1,
	/** Triangle button. */
	PSP_CTRL_TRIANGLE = -1,
	/** Circle button. */
	PSP_CTRL_CIRCLE = -1,
	/** Cross button. */
	PSP_CTRL_CROSS = -1,
	/** Square button. */
	PSP_CTRL_SQUARE = -1,
	/** Home button. In user mode this bit is set if the exit dialog is visible. */
	PSP_CTRL_HOME = -1,
	/** Hold button. */
	PSP_CTRL_HOLD = -1,
	/** Music Note button. */
	PSP_CTRL_NOTE = -1,
	/** Screen button. */
	PSP_CTRL_SCREEN = -1,
	/** Volume up button. */
	PSP_CTRL_VOLUP = -1,
	/** Volume down button. */
	PSP_CTRL_VOLDOWN = -1,
	/** Wlan switch up. */
	PSP_CTRL_WLAN_UP = -1,
	/** Remote hold position. */
	PSP_CTRL_REMOTE = -1,
	/** Disc present. */
	PSP_CTRL_DISC = -1,
	/** Memory stick present. */
	PSP_CTRL_MS = -1,
};
#endif
#include <string>
#include <glm/glm.hpp>

namespace Stardust::Utilities {

	bool KeyPressed(int key);
	bool KeyHold(int key);
	void updateInputs();

	/*
		Returns from -1 -> 1 of the intensity of the selection.
		If it is not a joystick, it will always return 1
	*/
	float KeyStrength(int key);

	void addActionKeyPair(std::string action, int key);
	void clearActionKeyPairs();
	void setActionKeyPair(std::string action, int key);

	/**
	* Bool - Held?
	* Float - Strength
	*/
	typedef void (*ActionHandler)(bool, float);

	void addActionHandler(std::string action, ActionHandler handler);
	void clearActionHandlers();

	std::string toString(int but);

	void LoadConfiguration(std::string path);
	void SaveConfiguration(std::string path);

	int buttonFromAction(std::string);

	int nextAction();

	glm::vec2 getCursorPos();
}