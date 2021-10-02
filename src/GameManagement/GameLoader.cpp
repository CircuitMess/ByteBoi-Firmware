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
	if(this->game != nullptr) return;
	this->game = game;
	done = false;
	error = "";
	progress = 0;

	loadTask.start(1, 0);
}

void GameLoader::loadFunc(Task* task){
	GameLoader* loader = static_cast<GameLoader*>(task->arg);
	GameInfo* game = loader->game;
	if(game == nullptr) return;

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

	if(loader->checkAbort()) return;

#define error(message) loader->error = message; loader->game = nullptr; return

	//copy resources
	if(!game->resources.empty() && SD.exists(game->resources.c_str())){
		root = SD.open(game->resources.c_str());
		file = root.openNextFile();
		while(file){
			String fileName = file.name();
			fileName = fileName.substring(fileName.lastIndexOf('/'));
			File destFile = SPIFFS.open(ByteBoiImpl::SPIFFSgameRoot + fileName, FILE_WRITE);
			if(!destFile){
				file.close();
				destFile.close();
				error(String("Failed opening SPIFFS destination: ") + ByteBoiImpl::SPIFFSgameRoot + fileName);
			}
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
	if(!file || file.isDirectory()){
		file.close();
		error(String("Error opening binary: ") + game->binary.c_str());
	}

	size_t updateSize = file.size();
	if(updateSize <= 0){
		file.close();
		error("Binary size is 0.");
	}

	if(!Update.begin(updateSize)){
		file.close();
		error("Binary too large.");
	}

	size_t totalWritten = 0;
	const size_t at_once = 64;
	uint8_t* buffer = static_cast<uint8_t*>(malloc(at_once));
	while(totalWritten < updateSize){
		size_t read = file.read(buffer, min((size_t) at_once, updateSize - totalWritten));
		if(read == 0){
			Update.abort();
			file.close();
			error("Error reading binary from SD card.");
		}

		if(loader->checkAbort()){
			Update.abort();
			file.close();
			return;
		}

		size_t written = Update.write(buffer, read);
		if(written != read){
			Update.abort();
			file.close();
			error("Error writing binary to flash.");
		}

		if(loader->checkAbort()){
			Update.abort();
			file.close();
			return;
		}

		totalWritten += written;
		loader->progress = (float) totalWritten / (float) updateSize;
		yield();
	}
	file.close();

	if(!Update.end(true)){
		error("Update finalization failed.");
	}

	if(!Update.isFinished()){
		error("Update not finished.");
	}

	esp_ota_set_boot_partition(esp_ota_get_running_partition());

	if(loader->checkAbort()) return;

	loader->done = true;

	printf("Update finished\n");
}

void GameLoader::boot(){
	esp_ota_set_boot_partition(esp_ota_get_next_update_partition(esp_ota_get_running_partition()));
	ESP.restart();
}

void GameLoader::abort(){
	game = nullptr;
	done = false;
	error = "";
	progress = 0;
}

bool GameLoader::doneLoading(){
	return done || error != "";
}

bool GameLoader::checkAbort(){
	return game == nullptr;
}

const String& GameLoader::getError() const{
	return error;
}

void GameLoader::clearError(){
	error = "";
}

float GameLoader::getProgress() const{
	return progress;
}
