#ifndef INPUTLISTENET_H
#define INPUTLISTENET_H

#include <windows.h>

class InputListener
{

public:
	virtual void keyDownEvent(WPARAM input) = 0;
	virtual void keyUpEvent(WPARAM input) = 0;

};

#endif