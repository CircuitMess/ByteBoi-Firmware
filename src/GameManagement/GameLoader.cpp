#include "GameLoader.h"
#include <Update.h>
#include <SD.h>
#include "../GameInfo.hpp"
#include <ByteBoi.h>
#include <SPIFFS.h>
#include <esp_ota_ops.h>

GameLoader Loader;

GameLoader::GameLoader() : loadTask("GameLoad", GameLoader::loadFunc, 4096, this){}

void GameLoader::loadGame(GameInfo* game){
	if(!ByteBoiImpl::inFirmware()) return;
	if(loading != nullptr) return;
	loading = game;
	done = false;

	loadTask.start(1, 0);
}

void GameLoader::loadFunc(Task* task){
	GameLoader* loader = static_cast<GameLoader*>(task->arg);

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

	GameInfo* game = loader->loading;
	if(game == nullptr) return;

	//copy resources
	if(!game->resources.empty() && SD.exists(game->resources.c_str())){
		root = SD.open(game->resources.c_str());
		file = root.openNextFile();
		while(file){
			String fileName = file.name();
			fileName = fileName.substring(fileName.lastIndexOf('/') + 1);
			File destFile = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot + fileName, FILE_WRITE);
			uint8_t buf[512];
			size_t count;
			while((count = file.read(buf, 512)) != 0){
				destFile.write(buf, count);
				yield();
			}
			destFile.close();
			file.close();

			if(loader->checkAbort()){
				root.close();
				return;
			}

			file = root.openNextFile();
		}
		file.close();
		root.close();
	}

	yield();

	if(loader->checkAbort()) return;

	file = SD.open(game->binary.c_str());
	if(!file || file.isDirectory()) return;

	size_t updateSize = file.size();
	if(updateSize <= 0){
		printf("Update size is <= 0\n");
		file.close();
		return;
	}

	if(!Update.begin(updateSize)){
		printf("Update begin failed, not enough space\n");
		file.close();
		return;
	}

	size_t totalWritten = 0;
	const size_t at_once = 64;
	uint8_t* buffer = static_cast<uint8_t*>(malloc(at_once));
	while(totalWritten < updateSize){
		size_t read = file.read(buffer, min((size_t) at_once, updateSize - totalWritten));
		if(read == 0){
			printf("Failed reading after %ld bytes\n", totalWritten);
			Update.abort();
			file.close();
			return;
		}

		if(loader->checkAbort()){
			Update.abort();
			file.close();
			return;
		}

		size_t written = Update.write(buffer, read);
		if(written != read){
			printf("Failed writing after %ld bytes\n", totalWritten);
			Update.abort();
			file.close();
			return;
		}

		if(loader->checkAbort()){
			Update.abort();
			file.close();
			return;
		}

		totalWritten += written;
		yield();
	}
	file.close();

	if(!Update.end(true)){
		printf("Update end failed\n");
		return;
	}

	if(!Update.isFinished()){
		printf("Update not finished\n");
		return;
	}

	esp_ota_set_boot_partition(esp_ota_get_running_partition());
	loader->done = true;

	printf("Update finished\n");
}

void GameLoader::boot(){
	esp_ota_set_boot_partition(esp_ota_get_next_update_partition(esp_ota_get_running_partition()));
	ESP.restart();
}

void GameLoader::abort(){
	loading = nullptr;
}

bool GameLoader::doneLoading(){
	return done;
}

bool GameLoader::checkAbort(){
	return loading == nullptr;
}
