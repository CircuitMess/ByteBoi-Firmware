#include "GameLoader.h"
#include <Update.h>
#include <SD.h>
#include "../GameInfo.hpp"
#include <ByteBoi.h>
#include <SPIFFS.h>
#include <esp_ota_ops.h>


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
				fileName = fileName.substring(fileName.lastIndexOf('/'));
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
			if (written == updateSize) {
				Serial.println("Written : " + String(written) + " successfully");
			}
			else {
				Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
			}
			if (Update.end()) {
				Serial.println("OTA done!");
				if (Update.isFinished()) {
					Serial.println("Update successfully completed. Rebooting.");
				}
				else {
					Serial.println("Update not finished? Something went wrong!");
				}
			}
			else {
				Serial.println("Error Occurred. Error #: " + String(Update.getError()));
			}

		}
		else
		{
			Serial.println("Not enough space to begin OTA");
		}
	}
	file.close();
	esp_ota_set_boot_partition(esp_ota_get_next_update_partition(esp_ota_get_running_partition()));
	ESP.restart();
}
