#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "../../../dependencies.h"

namespace nerv
{
	class texture
	{
	private:
		size_t id;
		int width;
		int height;
		int nbChannels;

		std::string name;
	public:

		texture();

		texture(std::string path);
		
		//getter
		inline int getWidth() { return this->width; }
		inline int getHeight() { return this->height; }
		inline int getnbChannels() { return this->nbChannels; }
		inline size_t getId() { return this->id; }

	};
}