#include "inputclass.h"


void InputClass::Init()
{

	for (int i = 0; i < 256; i++) mKeys[i] = false;

	return;

}

void InputClass::KeyDown(unsigned int input)
{

	mKeys[input] = true;
	
	return;

}

void InputClass::KeyUp(unsigned int input)
{

	mKeys[input] = false;

	return;

}

bool InputClass::IsKeyDown(unsigned int key)
{

	return mKeys[key];

}