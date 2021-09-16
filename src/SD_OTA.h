#ifndef BYTEBOI_SD_OTA_H
#define BYTEBOI_SD_OTA_H

#include <FS.h>

class SD_OTA {
public:
// check given FS for valid update.bin and perform update if available
	static void updateFromSD(const char* path);

private:
	// perform the actual update from a given stream
	static void performUpdate(Stream &updateSource, size_t updateSize);
};


#endif //BYTEBOI_SD_OTA_H
