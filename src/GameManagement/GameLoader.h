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
	const String& getError() const;
	void clearError();
	float getProgress() const;

	static void loadFunc(Task* task);

private:
	Task loadTask;
	GameInfo* game = nullptr;
	bool done = false;
	String error = "";
	float progress = 0;

	bool checkAbort();
};

extern GameLoader Loader;


#endif //BYTEBOI_FIRMWARE_GAMELOADER_H
