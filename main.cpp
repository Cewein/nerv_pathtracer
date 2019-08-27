#include "dependencies.h"
#include <iostream>

#include "src/in_out/windows/windows.h"

int main()
{
	logger.info("init", "start of a long journey");
	screen.create("nerv", 640,480, false);

	while (screen.isOpen()) {
		screen.update();
	}

	screen.close();
	return 0;
}