#ifndef BYTEBOI_FIRMWARE_GAMEMANAGER_H
#define BYTEBOI_FIRMWARE_GAMEMANAGER_H

#include <vector>
#include <FS.h>

struct GameInfo;

class GameManager {
public:
	void scanGames();
	const std::vector<GameInfo*> & getGames();
	GameInfo* getGame(int index);


private:
	std::vector<GameInfo*> games;
};

extern GameManager Games;
#endif //BYTEBOI_FIRMWARE_GAMEMANAGER_H
