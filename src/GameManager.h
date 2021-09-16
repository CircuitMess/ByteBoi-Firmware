#ifndef BYTEBOI_FIRMWARE_GAMEMANAGER_H
#define BYTEBOI_FIRMWARE_GAMEMANAGER_H

#include <vector>
#include <FS.h>
#include <Properties.h>

using namespace cppproperties;


class GameManager {
public:
	//functions used by the launcher/firmware
	static void loadGame(size_t index);
	static void scanGames();
	static const std::vector<Properties> &getGameProperties();
	static const char* getGameName(size_t index);
	static fs::File getIcon(size_t index);
	static const char* getGameBinary(size_t index);
	static const char* getGameResources(size_t index);

private:
	static std::vector<Properties> games;
};


#endif //BYTEBOI_FIRMWARE_GAMEMANAGER_H
