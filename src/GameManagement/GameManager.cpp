#include "GameManager.h"
#include <SPIFFS.h>
#include <SD.h>
#include <SPI.h>
#include <esp_partition.h>
#include <esp_ota_ops.h>
#include <PropertiesParser.h>
#include <iostream>
#include <utility>
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
				if(binaryPath == "") binaryPath = "firmware.bin";
				memset(path, 0, 100);
				strncat(path, gameFolder.name(), 100);
				strncat(path, "/", 100);
				strncat(path, binaryPath.c_str(), 100);
				if(SD.exists(path)){
					auto game = new GameInfo(GameInfo{
							getValueOrDefault(props, "Name", gameDefaults.name),
							props.GetProperty("Author"),
							props.GetProperty("Description"),
							getValueOrDefault(props, "Icon", gameDefaults.icon),
							getValueOrDefault(props, "Binary", gameDefaults.binary),
							getValueOrDefault(props, "Resources", gameDefaults.resources)
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
