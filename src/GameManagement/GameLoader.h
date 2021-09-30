#ifndef BYTEBOI_FIRMWARE_GAMELOADER_H
#define BYTEBOI_FIRMWARE_GAMELOADER_H

#include <Util/Task.h>
#include <FS.h>
struct GameInfo;

class GameLoader {
public:
	GameLoader();

// check given FS for valid update.bin and perform update if available
	void loadGame(GameInfo* game);
	void boot();
	void abort();
	bool doneLoading();

	static void loadFunc(Task* task);

private:
	Task loadTask;
	GameInfo* loading = nullptr;
	bool done;

	bool checkAbort();
};

extern GameLoader Loader;


#endif //BYTEBOI_FIRMWARE_GAMELOADER_H
