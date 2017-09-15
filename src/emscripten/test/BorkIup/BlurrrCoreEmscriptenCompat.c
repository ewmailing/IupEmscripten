
// Quick interface shim for Blurrr/Emscripten




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "BlurrrCore.h"


#include <stdarg.h>
void BlurrrLog_SysLog(const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vprintf(fmt, argp);
	puts("\n");
	va_end(argp);
}

bool BlurrrCore_Init()
{
	return true;
}

void BlurrrCore_Quit()
{
}

struct BlurrrFileHandle
{
	FILE* fileHandle;
};

// Not handling errors
struct BlurrrFileHandle* BlurrrFile_Open(const char* file_name, const char* mode)
{
	struct BlurrrFileHandle* blurrr_file_handle = NULL;
	FILE* fh = fopen(file_name, mode);
	if(NULL != fh)
	{
		blurrr_file_handle = (struct BlurrrFileHandle*)calloc(1, sizeof(struct BlurrrFileHandle));
		blurrr_file_handle->fileHandle = fh;
		return blurrr_file_handle;
	}
	else
	{
		printf("failed to open file\n");
		return NULL;
	}
}

size_t BlurrrFile_Read(struct BlurrrFileHandle* file_handle, char* data_buffer, size_t size, size_t nitems)
{
	size_t bytes_read = 0;
	if(NULL == file_handle)
	{
		return -1;
	}
	bytes_read = fread(data_buffer, size, nitems, file_handle->fileHandle);
    if(ferror(file_handle->fileHandle))
	{
		printf("Error reading from file.\n");
	}
	return bytes_read;
}
int BlurrrFile_Close(struct BlurrrFileHandle* file_handle)
{
	int ret_val;
	if(NULL == file_handle)
	{
		return 0;
	}

	ret_val = fclose(file_handle->fileHandle);
	if(0 != ret_val)
	{
		printf("Failed to close\n");
	}
	free(file_handle);
	return ret_val;
}


#include <sys/stat.h>
size_t BlurrrFile_Size(struct BlurrrFileHandle* file_handle)
{
	off_t file_size;
	char *buffer;
	struct stat st_buf;
	int fd;
	size_t ret_val;
	fd = fileno(file_handle->fileHandle); 
	if((fstat(fd, &st_buf) != 0) || (!S_ISREG(st_buf.st_mode)))
	{
		printf("Failed to get size\n");
	}

	ret_val = st_buf.st_size; 
	return ret_val;
}

void BlurrrPath_GetResourceDirectoryString(char* out_buffer, size_t max_buf_len)
{
//	out_buffer[0] = '/';
//	out_buffer[1] = '\0';
	//strlcpy(out_buffer, "asset_dir/", max_buf_len);
	strlcpy(out_buffer, "", max_buf_len);
}

 

