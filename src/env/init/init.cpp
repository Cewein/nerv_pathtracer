#include "init.h"

void nerv::init::launch()
{
	logger.info("init", "start of a long journey");
	std::vector<config::setting> conf = config::getSetting("config.ini", "window");
	window::get().create("window with config", conf[0].value, conf[1].value, conf[2].value);
	nerv::keyboard::init();
}
