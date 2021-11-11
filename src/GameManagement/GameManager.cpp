#include "GameManager.h"
#include <SPIFFS.h>
#include <SD.h>
#include <SPI.h>
#include <esp_partition.h>
#include <esp_ota_ops.h>
#include <PropertiesParser.h>
#include <ByteBoi.h>
#include <Properties.h>
#include "../GameInfo.hpp"
#include "GameListener.hpp"

GameManager Games;
using namespace cppproperties;

struct GameDefaultsStruct {
	const char* name = "Game";
	const char* icon = "icon.bmp";
	const char* binary = "firmware.bin";
	const char* resources = "resources";
} gameDefaults;


std::string getValueOrDefault(Properties& props, const char* key, const char* defaultValue){
	if(props.GetProperty(key, defaultValue).empty()){
		return defaultValue;
	}
	std::string s = props.GetProperty("Name", gameDefaults.name);
	s.erase(remove(s.begin(), s.end(), '\r'), s.end());
	return s;
}

void GameManager::scanGames(){
	gamesRescanned = true;
	clearGames();

	if((ByteBoi.getExpander()->getPortState() & (1 << SD_DETECT_PIN))) return;

	File root = SD.open("/");
	File gameFolder = root.openNextFile();
	while(gameFolder){
		if(gameFolder.isDirectory()){
			char path[100] = {0};
			strncat(path, gameFolder.name(), 100);
			strncat(path, "/game.properties", 100);

			if(SD.exists(path)){
				strncpy(path, "/sd", 100);
				strncat(path, gameFolder.name(), 100);
				strncat(path, "/game.properties", 100);

				GameInfo* game = parseInfo(path, gameFolder.name());
				if(game != nullptr){
					games.push_back(game);
				}
			}
		}
		gameFolder = root.openNextFile();
	}
	root.close();
	gameFolder.close();
}

GameInfo* GameManager::parseInfo(const char* infoFilePath, const char* dirName, bool checkBinary){
	Properties props = PropertiesParser::Read(infoFilePath);
	char path[100] = {0};
	std::string binaryPath = props.GetProperty("Binary", "");
	if(binaryPath.empty()) binaryPath = gameDefaults.binary;
	memset(path, 0, 100);
	strncat(path, dirName, 100);
	strncat(path, "/", 100);
	strncat(path, binaryPath.c_str(), 100);

	char resourcesPath[100] = {0};
	std::string resources = props.GetProperty("Resources", "");
	if(resources.empty()) resources = gameDefaults.resources;
	memset(resourcesPath, 0, 100);
	strncat(resourcesPath, dirName, 100);
	strncat(resourcesPath, "/", 100);
	strncat(resourcesPath, resources.c_str(), 100);

	char iconPath[100] = {0};
	std::string icon = props.GetProperty("Icon", "");
	if(icon.empty()) icon = gameDefaults.icon;
	memset(iconPath, 0, 100);
	strncat(iconPath, dirName, 100);
	strncat(iconPath, "/", 100);
	strncat(iconPath, icon.c_str(), 100);

	if(checkBinary && !SD.exists(path)) return nullptr;

	std::string author = props.GetProperty("Author", "");
	if(author == "CM Team") author = "CM team";

	return new GameInfo(GameInfo{
		getValueOrDefault(props, "Name", gameDefaults.name),
		author,
		props.GetProperty("Description", ""),
		(SD.exists(iconPath) ? iconPath : ""),
		path,
		(SD.exists(resourcesPath) ? resourcesPath : ""),
		dirName
	});
}

const std::vector<GameInfo*> & GameManager::getGames(){
	return games;
}

GameInfo* GameManager::getGame(int index){
	return games[index];
}

void GameManager::loop(uint){
	detectSD();
}

void GameManager::detectSD(){
	bool SDdetected = !(ByteBoi.getExpander()->getPortState() & (1 << SD_DETECT_PIN));
	if(SDdetected && !SDinsertedFlag){
		SD.begin(SD_CS, SPI);
		SDinsertedFlag = true;
		scanGames();
		if(listener == nullptr) return;
		listener->gamesChanged(SDinsertedFlag);
	}else if(!SDdetected && SDinsertedFlag){
		SD.end();
		SDinsertedFlag = false;
		clearGames();
		if(listener == nullptr) return;
		listener->gamesChanged(SDinsertedFlag);
	}
}

bool GameManager::SDinserted(){
	return SDinsertedFlag;
}

void GameManager::setGameListener(GameListener* listener_){
	listener = listener_;
}

void GameManager::clearGames(){
	for(auto game : games){
		delete game;
	}
	games.clear();
}

bool GameManager::isGamesRescanned() const{
	return gamesRescanned;
}

void GameManager::resetGamesRescanned(){
	gamesRescanned = 0;

}
