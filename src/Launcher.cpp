#include "Launcher.h"
#include <Input/Input.h>
#include "Splash.h"
#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include <Audio/Piezo.h>
#include "Bitmaps/battery.hpp"
#include <Loop/LoopManager.h>
#include <ByteBoi.h>
#include "GameManagement/GameManager.h"
#include "GameManagement/GameLoader.h"
#include "GameInfo.hpp"
#include "DescriptionModal.h"
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
	Games.setGameListener(this);
}

Launcher::~Launcher(){
	Games.setGameListener(nullptr);
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
	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;
		DescriptionModal* descriptionModal;
		descriptionModal = new DescriptionModal(*instance,instance->scroller->getSelectedGame());
		descriptionModal->push(instance);

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
//	canvas->setCursor(130, 10);
//	canvas->println(Battery.getPercentage());
//	canvas->println((1000000.0 / (float)drawTime1));
	screen.commit();
	drawTime1 = micros() - t;
}

void Launcher::draw(){
	screen.getSprite()->clear(C_HEX(0x0082ff));
	scroller->draw();
	title->draw();
	logo->draw();

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

}

void Launcher::deinit(){

}

void Launcher::gamesChanged(bool inserted){
	//TODO: implementirati loadanje/clearanje UI-a
}

