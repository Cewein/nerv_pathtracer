#pragma once
#include <fstream>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../dependencies.h"
namespace nerv 
{
	namespace file
	{
		std::string read(std::string path);
		int getSize(FILE * fp);
	}
}