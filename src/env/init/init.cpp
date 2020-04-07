#include "init.h"

void nerv::init::launch()
{
	logger.info("init", "start of a long journey");
	std::vector<nerv::config::setting> conf = nerv::config::getSetting("config.ini", "window");
	nerv::window::get().create("window with config", conf[0].value, conf[1].value, conf[2].value);

	logger.info("GPU INFO", std::string((char*)glGetString(GL_RENDERER)));
	logger.info("GPU INFO", std::string((char*)glGetString(GL_VERSION)));
	logger.info("GPU INFO", std::string((char*)glGetString(GL_VENDOR)) + '\n');

	nerv::keyboard::init();
}
