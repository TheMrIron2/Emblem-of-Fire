#include <map>
#include <Utilities/Input.h>
#include <Utilities/Timer.h>
#include <Math/MathFuncs.h>
#include <json/json.h>
#include <fstream>
#include <iostream>

namespace Stardust::Utilities {

	std::map<std::string, int> mymap;
	std::map<std::string, ActionHandler> handles;
#if CURRENT_PLATFORM == PLATFORM_PSP
	SceCtrlData oldPadData;
	SceCtrlData newPadData;

float getX()
{
	return (((float)newPadData.Lx - 122.5f) / 122.5f); //Range of +/- 1.0f
}

float getY()
{
	return (((float)newPadData.Ly - 122.5f) / 122.5f); //Range of +/- 1.0f
}
#endif 

	void updateInputs()
	{

#if CURRENT_PLATFORM == PLATFORM_PSP
		oldPadData = newPadData;
		sceCtrlReadBufferPositive(&newPadData, 1);
#endif
		//ON PC this is handled by Platform Update anyways.

		for (auto& [key, but] : mymap) {
			if (KeyHold(but) || KeyPressed(but)) {
				if(handles.find(key) != handles.end()){
					handles[key](KeyHold(but), KeyStrength(but));
				}
			}
		}

	}
	bool KeyPressed(int key)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		if (key == PSP_CTRL_ANALOG_X) {
			if (getX() > 0.3f || getX() < -0.3f) {
				return true;
			}
		}
		if (key == PSP_CTRL_ANALOG_Y) {
			if (getY() > 0.3f || getY() < -0.3f) {
				return true;
			}
		}

		if (newPadData.Buttons != oldPadData.Buttons)
		{
			if (newPadData.Buttons & key)
			{
				return true;
			}
		}
		return false;
#else
		//Filter out phony PSP keys
		if (key != -1) {
			return glfwGetKey(Platform::PC::g_Window->getWindow(), key) == GLFW_PRESS;
		}
		return false;
#endif
	}
	bool KeyHold(int key)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		if (key == PSP_CTRL_ANALOG_X) {
			if (getX() > 0.3f || getX() < -0.3f) {
				return true;
			}
		}
		if (key == PSP_CTRL_ANALOG_Y) {
			if (getY() > 0.3f || getY() < -0.3f) {
				return true;
			}
		}

		if (newPadData.Buttons & key)
		{
			return true;
		}

		return false;
#else
		return glfwGetKey(Platform::PC::g_Window->getWindow(), key) == GLFW_REPEAT;
#endif
	}
	float KeyStrength(int key)
	{

#if CURRENT_PLATFORM == PLATFORM_PSP
		if (key == PSP_CTRL_ANALOG_X) {
			return getX();
		}
		if (key == PSP_CTRL_ANALOG_Y) {
			return getY();
		}
#endif

		if (KeyHold(key) || KeyPressed(key)) {
			return 1.0f;
		}
		return 0.0f;

		if (KeyHold(key) || KeyPressed(key)) {
			return 1.0f;
		}
		return 0.0f;
	}


	void addActionKeyPair(std::string action, int key)
	{
		mymap.emplace(action, key);
	}
	
	void clearActionKeyPairs()
	{
		mymap.clear();
	}

	void setActionKeyPair(std::string action, int key)
	{
		mymap[action] = key;
	}
	
	void addActionHandler(std::string action, ActionHandler handler)
	{
		handles.emplace(action, handler);
	}

	void clearActionHandlers()
	{
		handles.clear();
	}


#if CURRENT_PLATFORM == PLATFORM_PSP
	std::string toString(int but)
	{
		switch(but){
		case PSP_CTRL_ANALOG_X:
			return "Analog X";

			break;

		case PSP_CTRL_ANALOG_Y:
			return "Analog Y";

			break;

		case PSP_CTRL_DOWN:
			return "Down";

			break;
		case PSP_CTRL_UP:
			return "Up";

			break;
		case PSP_CTRL_LEFT:
			return "Left";

			break;
		case PSP_CTRL_RIGHT:
			return "Right";

			break;


		case PSP_CTRL_CROSS:
			return "Cross";

			break;
		case PSP_CTRL_CIRCLE:
			return "Circle";

			break;
		case PSP_CTRL_TRIANGLE:
			return "Triangle";

			break;
		case PSP_CTRL_SQUARE:
			return "Square";

			break;


		case PSP_CTRL_RTRIGGER:
			return "RTrigger";

			break;
		case PSP_CTRL_LTRIGGER:
			return "LTrigger";

			break;
		case PSP_CTRL_SELECT:
			return "Select";

			break;
		case PSP_CTRL_START:
			return "Start";

			break;

	}

		return "Unbound";
	}
#endif
	void LoadConfiguration(std::string path)
	{
		mymap.clear();
		handles.clear();
		std::fstream file(path);

		Json::Value root;

		file >> root;
		file.close();

		Json::Value listen = root["listeners"];

		for (int i = 0; i < listen.size(); i++) {
			Json::Value temp = listen[i];
			std::string str = temp["name"].asString();
			int but = temp["button"].asInt();

			mymap.emplace(str, but);
		}
	}
	void SaveConfiguration(std::string path)
	{
		std::fstream file(path, std::ios::trunc);

		Json::Value v;

		Json::Value v2(Json::arrayValue);

		for (const auto& [str, but] : mymap) {
			Json::Value temp;
			temp["name"] = str.c_str();
			temp["button"] = but;

			v2.append(temp);
		}

		v["listeners"] = v2;

		file << v;
		file.close();
	}

	int buttonFromAction(std::string s){
		return mymap[s];
	}


#if CURRENT_PLATFORM == PLATFORM_PSP
	int nextAction()
{

	Utilities::Timer t;
	t = Utilities::Timer();

	float delay = 0.1f;

	delay = 1.0f;
	while (delay > 0) {
		delay -= t.deltaTime();
		updateInputs();
	}

	while (true) {
		updateInputs();

		//check each button
		if (KeyHold(PSP_CTRL_UP)) {
			return PSP_CTRL_UP;
		}

		if (KeyHold(PSP_CTRL_DOWN)) {
			return PSP_CTRL_DOWN;
		}
		if (KeyHold(PSP_CTRL_LEFT)) {
			return PSP_CTRL_LEFT;
		}
		if (KeyHold(PSP_CTRL_RIGHT)) {
			return PSP_CTRL_RIGHT;
		}

		if (KeyHold(PSP_CTRL_TRIANGLE)) {
			return PSP_CTRL_TRIANGLE;
		}
		if (KeyHold(PSP_CTRL_CROSS)) {
			return PSP_CTRL_CROSS;
		}
		if (KeyHold(PSP_CTRL_SQUARE)) {
			return PSP_CTRL_SQUARE;
		}
		if (KeyHold(PSP_CTRL_CIRCLE)) {
			return PSP_CTRL_CIRCLE;
		}

		if (KeyHold(PSP_CTRL_LTRIGGER)) {
			return PSP_CTRL_LTRIGGER;
		}
		if (KeyHold(PSP_CTRL_RTRIGGER)) {
			return PSP_CTRL_RTRIGGER;
		}
		if (KeyHold(PSP_CTRL_SELECT)) {
			return PSP_CTRL_SELECT;
		}
		if (KeyHold(PSP_CTRL_START)) {
			return PSP_CTRL_START;
		}

		if (getX() >= 0.5f) {
			return (int)PSP_CTRL_ANALOG_X;
		}
		if (getY() >= 0.5f) {
			return (int)PSP_CTRL_ANALOG_Y;
		}
		if (getX() <= -0.5f) {
			return (int)PSP_CTRL_ANALOG_X;
		}
		if (getY() <= -0.5f) {
			return (int)PSP_CTRL_ANALOG_Y;
		}
	}

	return (int)0;
}
#endif
	glm::vec2 getCursorPos()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		return glm::vec2(-1, -1);
#else

		double curX, curY;
		curX = curY = -1.0;

		glfwGetCursorPos(Platform::PC::g_Window->getWindow(), &curX, &curY);

		return glm::vec2((float)curX, (float)curY);
#endif
	}
}