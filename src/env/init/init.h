#pragma once
#include <vector>
#include <iostream>
#include <string>

#include "../../../dependencies.h"
#include "../../in_out/windows/windows.h"
#include "../../in_out/input/keyboard.h"
#include "../../config/config.h"
#include "../../engine/ui.h"

namespace nerv{
	namespace init{
		//lauch the application and load config
		//as of now it's a poor launcher function
		//but do the job
		void launch();
	};
}