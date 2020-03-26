#pragma once
#include "../../../dependencies.h"
#include "../material/texture.h"
#include "../../in_out/windows/windows.h"

namespace nerv
{
	class framebuffer
	{
	public:
		size_t id;
		
		//renderbuffer objet id
		size_t rbo;

		//texture where the frame buffer is drawn
		nerv::texture * frameTexture;

		framebuffer();
		~framebuffer();
	};
}