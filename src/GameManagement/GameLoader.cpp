#include "GameLoader.h"
#include <Update.h>
#include <SD.h>
#include "../GameInfo.hpp"
#include <ByteBoi.h>
#include <SPIFFS.h>

void GameLoader::loadGame(GameInfo* game){

	if(!ByteBoiImpl::inFirmware()) return;

	//create game folder if not present
	if(!SPIFFS.exists(ByteBoiImpl::SPIFFSgameRoot)){
		SPIFFS.mkdir(ByteBoiImpl::SPIFFSgameRoot);
	}
	//erase folder
	File root = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot);
	File file = root.openNextFile();
	while(file){
		SPIFFS.remove(file.name());
		file = root.openNextFile();
	}
	file.close();
	root.close();

	//copy resources
	char path[100];
	strncpy(path, "/", 100);
	strncat(path, game->name.c_str(), 100);
	strncat(path, "/", 100);
	strncat(path, game->resources.c_str(), 100);
	if(SD.exists(path)){
		root = SD.open(path);
		file = root.openNextFile();
		while(file){
			String fileName = file.name();
			fileName = fileName.substring(fileName.lastIndexOf('/') + 1);
			File destFile = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot + fileName, FILE_WRITE);
			uint8_t buf[512];
			while(file.available()){
				size_t readBytes = file.read(buf, 512);
				destFile.write(buf, readBytes);
			}
			destFile.close();
			file = root.openNextFile();
		}
		file.close();
		root.close();
	}


	strncpy(path, "/", 100);
	strncat(path, game->name.c_str(), 100);
	strncat(path, "/", 100);
	strncat(path, game->binary.c_str(), 100);

	file = SD.open(path);
	if(!file) return;
	if(file.isDirectory()){
		file.close();
		return;
	}

	size_t updateSize = file.size();
	if (updateSize > 0) {
		if (Update.begin(updateSize)) {
			size_t written = Update.writeStream(file);
		}
	}
	file.close();
	ESP.restart();
}
