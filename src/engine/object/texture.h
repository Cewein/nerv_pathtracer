#include <vector>
#include <string>

#include "../../../dependencies.h"

namespace nerv
{
	class texture
	{
	private:
		int width;
		int height;
		int nbChannels;

		std::string path;
		std::string name;

		std::vector<unsigned char> data;

	public:

		texture();

		texture(std::string path);
		
		//getter
		inline int getWidth() { return this->width; }
		inline int getHeight() { return this->height; }
		inline int getnbChannels() { return this->nbChannels; }

	};
}