#pragma once

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{

public:

	InputClass() = default;
	InputClass(const InputClass&) = delete;
	~InputClass() = default;

	void Init();
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:

	bool mKeys[256];

};

#endif // !_INPUTCLASS_H_
