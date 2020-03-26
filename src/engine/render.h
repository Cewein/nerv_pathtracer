#include "../../dependencies.h"
#include "object/framebuffer.h"

namespace nerv
{
	namespace render
	{
		void mainpass(nerv::framebuffer * framebuffer = nullptr);
		void postpass();
	}
}