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
	for(auto game : games){
		delete game;
	}
	games.clear();
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

				Properties props = PropertiesParser::Read(path);
				std::string binaryPath = props.GetProperty("Binary");
				if(binaryPath.empty()) binaryPath = gameDefaults.binary;
				memset(path, 0, 100);
				strncat(path, gameFolder.name(), 100);
				strncat(path, "/", 100);
				strncat(path, binaryPath.c_str(), 100);

				char resourcesPath[100] = {0};
				std::string resources = props.GetProperty("Resources");
				if(resources.empty()) resources = gameDefaults.resources;
				memset(resourcesPath, 0, 100);
				strncat(resourcesPath, gameFolder.name(), 100);
				strncat(resourcesPath, "/", 100);
				strncat(resourcesPath, resources.c_str(), 100);

				char iconPath[100] = {0};
				std::string icon = props.GetProperty("Icon");
				if(icon.empty()) icon = gameDefaults.icon;
				memset(iconPath, 0, 100);
				strncat(iconPath, gameFolder.name(), 100);
				strncat(iconPath, "/", 100);
				strncat(iconPath, icon.c_str(), 100);

				if(SD.exists(path)){
					auto game = new GameInfo(GameInfo{
							getValueOrDefault(props, "Name", gameDefaults.name),
							props.GetProperty("Author"),
							props.GetProperty("Description"),
							(SD.exists(iconPath) ? iconPath : ""),
							path,
							(SD.exists(resourcesPath) ? resourcesPath : "")
					});
					games.push_back(game);
				}
			}
		}
		gameFolder = root.openNextFile();
	}
	root.close();
	gameFolder.close();
}

const std::vector<GameInfo*> & GameManager::getGames(){
	return games;
}

GameInfo* GameManager::getGame(int index){
	return games[index];
}
