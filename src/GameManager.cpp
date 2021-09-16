#include "GameManager.h"
#include <SPIFFS.h>
#include "SD_OTA.h"
#include <SD.h>
#include <SPI.h>
#include <esp_partition.h>
#include <esp_ota_ops.h>
#include <PropertiesParser.h>
#include <iostream>
#include <utility>
#include <ByteBoi.h>

struct GameDefaultsStruct {
	const char* name = "Game";
	const char* icon = "icon.bmp";
	const char* binary = "firmware.bin";
	const char* resources = "resources";
} gameDefaults;

std::vector<Properties> GameManager::games;
using namespace cppproperties;

void GameManager::loadGame(size_t index){
	if(!ByteBoiImpl::inFirmware()) return;
	if(index >= games.size()) return;

	if(!SPIFFS.exists(ByteBoiImpl::SPIFFSgameRoot)){
		SPIFFS.mkdir(ByteBoiImpl::SPIFFSgameRoot);
	}
	File root = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot);
	File file = root.openNextFile();

	while(file){
		SPIFFS.remove(file.name());
		file = root.openNextFile();
	}
	file.close();
	root.close();

	char path[100];
	strncpy(path, "/", 100);
	strncat(path, getGameName(index), 100);
	strncat(path, "/", 100);
	strncat(path, getGameResources(index), 100);
	if(SD.exists(path)){
		root = SD.open(path);
		file = root.openNextFile();
		while(file){
			String fileName = file.name();
			fileName = fileName.substring(fileName.lastIndexOf('/') + 1);
			File destFile = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot + fileName, FILE_WRITE);
			uint8_t buf[512];
			while(file.read(buf, 512)){
				destFile.write(buf, 512);
			}
			destFile.close();
			file = root.openNextFile();
		}
		file.close();
		root.close();
	}
	memset(path, 0, 100);
	strncpy(path, "/", 100);
	strncat(path, getGameName(index), 100);
	strncat(path, "/", 100);
	strncat(path, getGameBinary(index), 100);

	SD_OTA::updateFromSD(path);
}

void GameManager::scanGames(){
	games.clear();
	File root = SD.open("/");
	File gameFolder = root.openNextFile();
	size_t counter = 0;
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
					games.push_back(props);
				}
			}
		}
		gameFolder = root.openNextFile();
		counter++;
	}
	root.close();
	gameFolder.close();
}

fs::File GameManager::getIcon(size_t index){
	if(index >= games.size()) return fs::File();

	char path[100] = {0};
	strncat(path, "/", 100);
	strncat(path, getGameName(index), 100);
	strncat(path, "/", 100);
	if(games[index].GetProperty("Icon", gameDefaults.icon).empty()){
		strncat(path, gameDefaults.icon, 100);
	}else{
		strncat(path, games[index].GetProperty("Icon", gameDefaults.icon).c_str(), 100);
	}
	return SD.open(path);
}

const std::vector<Properties> &GameManager::getGameProperties(){
	return games;
}

const char* GameManager::getGameName(size_t index){
	if(index >= games.size()) return nullptr;
	if(games[index].GetProperty("Name", gameDefaults.name).empty()){
		return gameDefaults.name;
	}
	return games[index].GetProperty("Name", gameDefaults.name).c_str();
}

const char* GameManager::getGameBinary(size_t index){
	if(index >= games.size()) return nullptr;
	if(games[index].GetProperty("Binary", gameDefaults.binary).empty()){
		return gameDefaults.binary;
	}
	return games[index].GetProperty("Binary", gameDefaults.binary).c_str();
}

const char* GameManager::getGameResources(size_t index){
	if(index >= games.size()) return nullptr;
	if(games[index].GetProperty("Resources", gameDefaults.resources).empty()){
		return gameDefaults.resources;
	}
	return games[index].GetProperty("Resources", gameDefaults.resources).c_str();
}
