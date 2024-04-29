#pragma once

#include <stdio.h>
#include "types.hpp"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

i64 file_size(FILE *file_ptr)
{
	if(fseek(file_ptr, 0, SEEK_END) == 0)
	{
		i64 buffer_size = ftell(file_ptr);

		if(fseek(file_ptr, 0, SEEK_SET) != 0)
		{
			return -1;
		}

		return buffer_size;
	}

	return -1;
};