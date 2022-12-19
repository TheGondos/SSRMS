#include "MMUTapeImage.h"
/*
MMUTapeImage::MMUTapeImage()
{
	tape_image = NULL;
}

MMUTapeImage::MMUTapeImage(const std::string &filename)
{
	tape_image = NULL;
	Open(filename);
}

bool MMUTapeImage::Open(const std::string &filename)
{
	if(tape_image == NULL)
	{
		overlapped.hEvent = NULL;
		overlapped.Internal = NULL;
		overlapped.Offset = 0;
		overlapped.OffsetHigh = 0;
		overlapped.Pointer = NULL;

		tape_image = CreateFile(filename.c_str(), (GENERIC_READ | GENERIC_WRITE), 
			FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}


	return false;
}

bool MMUTapeImage::OperationFinished()
{
	return (overlapped.Internal != STATUS_PENDING);
}
*/
