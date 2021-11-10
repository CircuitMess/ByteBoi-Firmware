#ifndef BYTEBOI_FIRMWARE_GAMEMANAGER_H
#define BYTEBOI_FIRMWARE_GAMEMANAGER_H

#include <vector>
#include <FS.h>
#include <Loop/LoopListener.h>

struct GameInfo;
class GameListener;

class GameManager : public LoopListener {
public:
	void scanGames();
	const std::vector<GameInfo*> & getGames();
	GameInfo* getGame(int index);

	void loop(uint) override;
	bool SDinserted();
	void detectSD();
	void setGameListener(GameListener* listener);

	GameInfo* parseInfo(const char* infoFilePath, const char* dirName, bool checkBinary = true);

	bool isGamesRescanned() const;
	void resetGamesRescanned();

private:
	std::vector<GameInfo*> games;
	bool SDinsertedFlag = false;
	GameListener* listener = nullptr;
	void clearGames();

	bool gamesRescanned = false;
};

extern GameManager Games;
#endif //BYTEBOI_FIRMWARE_GAMEMANAGER_H
