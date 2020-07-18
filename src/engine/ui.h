#pragma once

#include "../../dependencies.h"
#include "object/camera.h"


namespace nerv 
{
	namespace ui
	{
		void init();
		void newFrame();
		void draw(nerv::camera * cam);
		void clean();
	}
}