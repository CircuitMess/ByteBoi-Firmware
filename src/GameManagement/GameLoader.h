#ifndef BYTEBOI_FIRMWARE_GAMELOADER_H
#define BYTEBOI_FIRMWARE_GAMELOADER_H

#include <Util/Task.h>
#include <FS.h>
struct GameInfo;

class GameLoader {
public:
	GameLoader();

	class Job {
	friend GameLoader;
	public:
		void abort();
		bool isDone();
		bool hasError();
		String getError();
		float getProgress();

	private:
		bool done = false;
		bool aborted = false;
		String error = "";
		float progress = 0;
		GameInfo* game;
	};

// check given FS for valid update.bin and perform update if available
	Job* loadGame(GameInfo* game);
	void boot();
	Job* getCurrent();

	static void loadFunc(Task* task);

	GameInfo* getLoaded();
	bool hasLoaded();
	void checkLoaded();

private:
	Task loadTask;

	Job* current = nullptr;

	static bool checkAbort(Job* job);
	bool saveLoaded(GameInfo* game);
	void clearLoaded();

	static const char* loadedImage;
	static const char* loadedInfo;

	GameInfo* loadedGame = nullptr;
};

extern GameLoader Loader;


#endif //BYTEBOI_FIRMWARE_GAMELOADER_H
