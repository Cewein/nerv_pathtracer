#include "file.h"

std::string nerv::file::read(std::string path)
{
	logger.warning("FILE", "Openning file at " + path);

	FILE * fp = fopen(path.c_str(), "rb");
	if (fp)
	{
		char * buffer = (char*)malloc(sizeof(char) * 250);
		char * fileArray = (char*)calloc(1, getSize(fp) + 1); //we calloc because we need a empty array (or at least filled with 0)

		while (fgets(buffer, 250, fp) != NULL)
		{
			strcat(fileArray, buffer);
		}

		free(buffer);

		return fileArray;
	}
	else
	{
		logger.warning("FILE", "The file open and been read successfully");
		logger.error("FILE", "The file did not open, check if it exist or if the path is right");
		return NULL;
	}
}

int nerv::file::getSize(FILE * fp)
{
	long bytes;
	fseek(fp, 0, SEEK_END);
	bytes = ftell(fp);
	rewind(fp);
	return bytes;
}
