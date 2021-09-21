#ifndef BYTEBOI_FIRMWARE_GAMELOADER_H
#define BYTEBOI_FIRMWARE_GAMELOADER_H

#include <FS.h>
struct GameInfo;

class GameLoader {
public:
// check given FS for valid update.bin and perform update if available
	static void loadGame(GameInfo* game);
};


#endif //BYTEBOI_FIRMWARE_GAMELOADER_H
