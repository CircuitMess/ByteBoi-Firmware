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

	if(!game->resources.empty()){
		//copy resources
		if(SD.exists(game->resources.c_str())){
			root = SD.open(game->resources.c_str());
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
	}

	file = SD.open(game->binary.c_str());
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
