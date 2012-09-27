#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <windows.h>
#include <vector>
#include "InputListener.h"

//class InputListener;

class InputManager
{
public: 
	static InputManager* GetInstance();

	bool registerInput(WPARAM input);
	void unRegisterInput(WPARAM input);
	void handleInput(WPARAM input, bool Down);
	void setListener(InputListener* listener);

private:

	InputManager();

	static InputManager* m_InputManager;
	InputListener* m_Listener;
	std::vector<WPARAM> m_RegisterInput;


};

#endif