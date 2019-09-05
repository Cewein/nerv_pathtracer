#include "dependencies.h"

#include "src/in_out/windows/windows.h"
#include "src/in_out/input/keyboard.h"
#include "src/config/config.h"

int main()
{
	logger.info("init", "start of a long journey");
	nerv::window::get().create("nerv", 640,480, false);
	nerv::keyboard::init();

	std::vector<nerv::config::setting> keyboard = nerv::config::getSetting("config.txt", "key");

	std::cout << "looking if config is properly loaded" << std::endl;
	for (int i = 0; i < keyboard.size(); i++)
		std::cout << keyboard[i].name << " : " << keyboard[i].value << std::endl;

	while (nerv::window::get().isOpen()) {
		nerv::window::get().update();
	}

	nerv::window::get().close();
	return EXIT_SUCCESS;
}