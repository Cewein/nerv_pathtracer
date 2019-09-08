
#include "dependencies.h"
#include "src/engine.h"


int main()
{
	nerv::init::launch();

	while (nerv::window::get().isOpen()) {
		nerv::window::get().update();
	}

	nerv::window::get().close();
	return EXIT_SUCCESS;
}