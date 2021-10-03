#include "GameLoader.h"
#include <Update.h>
#include <SD.h>
#include "../GameInfo.hpp"
#include <ByteBoi.h>
#include <SPIFFS.h>
#include <esp_ota_ops.h>

GameLoader Loader;

GameLoader::GameLoader() : loadTask("GameLoad", GameLoader::loadFunc, 4096, this){}

void GameLoader::Job::abort(){
	aborted = true;
}

bool GameLoader::Job::isDone(){
	return done;
}

bool GameLoader::Job::hasError(){
	return error != "";
}

String GameLoader::Job::getError(){
	return error;
}

float GameLoader::Job::getProgress(){
	return progress;
}

GameLoader::Job* GameLoader::loadGame(GameInfo* game){
	if(!ByteBoiImpl::inFirmware()) return nullptr;
	if(current){
		if(current->done || current->aborted){
			// Wait for task to finish if it's still running
			while(loadTask.running){
				delay(1);
			}

			delete current;
		}else{
			printf("Another load operation pending\n");
			return nullptr;
		}
	}

	current = new Job;
	current->game = game;

	loadTask.start(1, 0);
	return current;
}

void GameLoader::loadFunc(Task* task){
	GameLoader* loader = static_cast<GameLoader*>(task->arg);
	Job* job = loader->current;

	if(job == nullptr){
		printf("Update task started, current job is nullptr.\n"); // Memory leak. Game starts loading, stopped before task gets access to update
		return;
	}

	GameInfo* game = job->game;
	if(game == nullptr || checkAbort(job)) return;

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

	if(checkAbort(job)) return;

#define error(message) printf("E: %s\n", String(message).c_str()); job->error = message; job->done = true; return

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

			if(checkAbort(job)){
				root.close();
				return;
			}

			file = root.openNextFile();
		}
		file.close();
		root.close();
	}

	yield();

	if(checkAbort(job)) return;

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
	const size_t at_once = 512;
	uint8_t* buffer = static_cast<uint8_t*>(malloc(at_once));
	while(totalWritten < updateSize){
		size_t read = file.read(buffer, min((size_t) at_once, updateSize - totalWritten));
		if(read == 0){
			Update.abort();
			file.close();
			free(buffer);
			error("Error reading binary from SD card.");
		}

		if(checkAbort(job)){
			Update.abort();
			file.close();
			free(buffer);
			return;
		}

		size_t written = Update.write(buffer, read);
		if(written != read){
			Update.abort();
			file.close();
			free(buffer);
			error("Error writing binary to flash.");
		}

		if(checkAbort(job)){
			Update.abort();
			file.close();
			free(buffer);
			return;
		}

		totalWritten += written;
		job->progress = (float) totalWritten / (float) updateSize;
		yield();
	}
	file.close();
	free(buffer);

	if(!Update.end(true)){
		error("Update finalization failed.");
	}

	if(!Update.isFinished()){
		error("Update not finished.");
	}

	esp_ota_set_boot_partition(esp_ota_get_running_partition());

	if(checkAbort(job)) return;

	job->done = true;
	if(job->aborted){
		printf("Update aborted after being done.\n"); // Possible memory leak
	}

	printf("Update finished\n");
}

void GameLoader::boot(){
	esp_ota_set_boot_partition(esp_ota_get_next_update_partition(esp_ota_get_running_partition()));
	ESP.restart();
}

bool GameLoader::checkAbort(Job* job){
	if(job->aborted){
		job->done = true;
		return true;
	}

	return false;
}

