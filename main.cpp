#include "dependencies.h"
#include <iostream>

#include "src/in_out/windows/windows.h"

int main()
{
	logger.info("init", "start of a long journey");
	screen.create();
	while (screen.isOpen()) {
		glfwSwapBuffers(screen.display());
		glfwPollEvents();
	}
	screen.end();
	return 0;
}