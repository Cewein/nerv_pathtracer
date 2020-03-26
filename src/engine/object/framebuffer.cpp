#include "framebuffer.h"


nerv::framebuffer::framebuffer()
{
	logger.warning("FRAMEBUFFER", "Creating frame buffer");

	glGenFramebuffers(1, &(this->id));
	glBindFramebuffer(GL_FRAMEBUFFER, this->id); //this make a offscreen render of the buffer set to 0 for onscreen rendering
	this->frameTexture = new nerv::texture(nerv::window::get().width, nerv::window::get().height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->frameTexture->getId(), 0);
	logger.info("FRAMEBUFFER", "Created texture");

	glGenRenderbuffers(1, &(this->rbo));
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->frameTexture->getWidth(), this->frameTexture->getHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	logger.info("FRAMEBUFFER", "alloced space");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		logger.error("FRAMEBUFFER", "creation did not succed");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		logger.warning("FRAMEBUFFER", "creation succed");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

nerv::framebuffer::~framebuffer()
{
	glDeleteBuffers(1, &(this->id));
	delete(this->frameTexture);
}