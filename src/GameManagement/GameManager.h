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
	void setGameListener(GameListener* listener);

private:
	std::vector<GameInfo*> games;
	bool SDinsertedFlag = true;
	GameListener* listener = nullptr;
	void clearGames();
};

extern GameManager Games;
#endif //BYTEBOI_FIRMWARE_GAMEMANAGER_H
