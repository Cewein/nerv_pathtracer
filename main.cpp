#include "dependencies.h"
#include <iostream>

#include "src/in_out/windows/windows.h"
#include "src/in_out/input/keyboard.h"

int main()
{
	logger.info("init", "start of a long journey");
	nerv::screen.create("nerv", 640,480, false);
	nerv::keyboard::init();

	while (nerv::screen.isOpen()) {
		nerv::screen.update();
	}

	nerv::screen.close();
	return 0;
}