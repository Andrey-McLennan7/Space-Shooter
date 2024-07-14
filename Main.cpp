#include "Application.h"

#include <vector>

#include <iostream>
#include <ctime>

#undef main

int main()
{
	srand(time(0));

	Application* application{ new Application };

	if (application->init())
	{
		application->loop();
	}

	delete application;

	return 0;
}