#include "Launcher.h"
#include <Input/Input.h>
#include "Splash.h"
#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include <Audio/Piezo.h>
#include <Loop/LoopManager.h>
#include <ByteBoi.h>
#include "GameManagement/GameManager.h"
#include "GameManagement/GameLoader.h"
#include "GameInfo.hpp"
#include <SD.h>
#include <SPIFFS.h>
#include <FS/CompressedFile.h>

Launcher* Launcher::instance = nullptr;

Launcher::Launcher(Display* display) : Context(*display), display(display){
	canvas = screen.getSprite();
	scroller = new GameScroller(canvas);
	logo = new Logo(canvas);
	title = new GameTitle(canvas);

	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);
	splash = new Splash(display->getBaseSprite(), logo, title, scroller);

	Launcher::pack();
}

Launcher::~Launcher(){
	free(backgroundBuffer);
	free(logoBuffer);
}

void Launcher::start(){
	if(splash == nullptr){
		bindInput();
	}
	draw();
	screen.commit();
	LoopManager::addListener(this);
}

void Launcher::stop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
}

void Launcher::prev(){
	uint8_t selecting = instance->scroller->prev();
	if(selecting != selectedGame){
		instance->title->change(Games.getGame(selecting)->name.c_str());
	}
	selectedGame = selecting;
}

void Launcher::next(){
	uint8_t selecting = instance->scroller->next();
	if(selecting != selectedGame){
		instance->title->change(Games.getGame(selecting)->name.c_str());
	}
	selectedGame = selecting;
}

void Launcher::bindInput(){
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		instance->next();
		Piezo.tone(800, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		instance->prev();
		Piezo.tone(800, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance->scroller->scrolling()) return;
		GameLoader::loadGame(Games.getGame(instance->selectedGame));
	});
}

uint32_t drawTime1 = 0;

void Launcher::loop(uint _micros){

	int t = micros();
	if(splash){
		splash->loop(_micros);

		if(splash->done()){
			delete splash;
			splash = nullptr;

			bindInput();
			scroller->splash(1);
			title->change(Games.getGame(selectedGame)->name.c_str());
		}
	}else{
		logo->loop(_micros);
	}

	draw();
//	canvas->setTextColor(TFT_WHITE);
//	canvas->setTextSize(1);
//	canvas->setCursor(120, 10);
//	canvas->println(GameManager::getExpander()->pinRead(8));
//	canvas->setCursor(100, 10);
//	canvas->println(GameManager::getExpander()->pinRead(10));
//	canvas->setCursor(130, 10);
//	canvas->println(analogRead(36));
//	canvas->println((1000000.0 / (float)drawTime1));
	screen.commit();
	drawTime1 = micros() - t;
}

void Launcher::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 120, 1);
	scroller->draw();
	title->draw();
	//logo->draw();
	screen.getSprite()->drawIcon(logoBuffer, 35, 5, 93, 26, 1);

/*
	if(batteryService->getVoltage() > 780){
		canvas->drawBitmap(screen.getWidth() - 8, 0, battery1, 8, 12, TFT_WHITE);
	}else if(batteryService->getVoltage() <= 780 && batteryService->getVoltage() >= 700){
		canvas->drawBitmap(screen.getWidth() - 8, 0, battery2, 8, 12, TFT_WHITE);
	}else if(batteryService->getVoltage() < 700){
		canvas->drawBitmap(screen.getWidth() - 8, 0, battery3, 8, 12, TFT_WHITE);
	}
*/

}

void Launcher::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 120 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Launcher/mainMenuBg.raw.hs"), 13, 12);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 120 * 2);
	backgroundFile.close();
	logoBuffer = static_cast<Color*>(ps_malloc(93 * 26 * 2));
	if(logoBuffer == nullptr){
		Serial.printf("Logo picture unpack error\n");
		return;
	}

	fs::File iconFile = SPIFFS.open("/Launcher/ByteBoiLogo.raw");

	iconFile.read(reinterpret_cast<uint8_t*>(logoBuffer), 93 * 26 * 2);
	iconFile.close();
}

void Launcher::deinit(){
	free(backgroundBuffer);
	free(logoBuffer);
}

