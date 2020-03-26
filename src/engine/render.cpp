#include "render.h"

void nerv::render::mainpass(nerv::framebuffer * framebuffer)
{
	
	if (framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	
}

void nerv::render::postpass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}
