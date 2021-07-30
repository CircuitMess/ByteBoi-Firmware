#include "Launcher.h"
#include "Pins.hpp"

#include <Input/Input.h>

#include "Splash.h"
#include "GameScroller.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
//#include "Games/Bonk/GameInfo.hpp"
//#include "Games/SpaceRocks/GameInfo.hpp"
//#include "Games/Snake/GameInfo.hpp"
//#include "Games/Invaderz/GameInfo.hpp"
#include "SettingsMenu/GameInfo.hpp"
#include "../GameInfo.hpp"
#include "Services/BatteryService.h"
#include <Audio/Piezo.h>
#include "Menu.h"
#include "Bitmaps/battery.hpp"
#include "Services/SleepService.h"

Context* runningContext = nullptr;
bool exitingGame = false;

const GameInfo games[] = {
	//InvaderzInfo, BonkInfo, SpaceRocksInfo, SnakeInfo,
	SettingsInfo
};

Launcher* instance = nullptr;

Launcher::Launcher(Display* display, BatteryService* batteryService) : Context(*display), batteryService(batteryService), display(display)
{
	canvas = display->getBaseSprite();
	if(canvas == nullptr){
	}

	scroller = new GameScroller(canvas, games, sizeof(games) / sizeof(GameInfo));
	logo = new Logo(canvas);
	title = new GameTitle(canvas);


	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);
	splash = new Splash(display->getBaseSprite(), logo, title, scroller);
	menu = new Menu(*display);
	SleepService::getInstance()->addOnSleepCallback([](){
		instance->menu->stop(true);
	});

	BatteryService::getInstance()->setModalCallback([](){
		instance->menu->stop(true);
	});
}

void Launcher::start(){
	if(runningContext != nullptr && runningContext != this){
		delete runningContext;
	}

	exitingGame = false;
	runningContext = this;
	if(splash == nullptr){
		bindInput();
	}
	draw();
	screen.commit();
	LoopManager::addListener(this);
}

void Launcher::stop()
{
	LoopManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
}

void Launcher::prev(){
	uint8_t selecting = instance->scroller->prev();
	if(selecting != selectedGame){
		instance->title->change(games[selecting].title);
	}
	selectedGame = selecting;
}

void Launcher::next(){
	uint8_t selecting = instance->scroller->next();
	if(selecting != selectedGame){
		instance->title->change(games[selecting].title);
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

		Display* display = instance->display;
		uint8_t index = instance->selectedGame;

		Context* game = games[index].launch(*display);
		game->push(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(exitingGame || runningContext == nullptr || runningContext == instance || BatteryService::getInstance()->modalShown()) return;

		instance->menu->toggle(runningContext);
		Piezo.tone(500, 50);
	});
}
uint32_t drawTime1 = 0;

void Launcher::loop(uint _micros){
	uint32_t t = micros();
	if(splash){
		splash->loop(_micros);

		if(splash->done()){
			Serial.println("splash done");
			delete splash;
			splash = nullptr;

			bindInput();
			scroller->splash(1);
			title->change(games[selectedGame].title);
		}
	}else{
		logo->loop(_micros);
	}

	draw();
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextSize(1);
	canvas->setCursor(130, 10);
	canvas->println((1000000.0 / (float)drawTime1));
	screen.commit();
	drawTime1 = micros() - t;
}

void Launcher::draw(){
	canvas->clear(TFT_BLACK);
	scroller->draw();
	title->draw();
	logo->draw();

	if(batteryService->getVoltage() > 780)
	{
		canvas->drawMonochromeIcon(battery1, 120, 0, 8, 12, 1, TFT_WHITE);
	}
	else if(batteryService->getVoltage() <= 780 && batteryService->getVoltage() >= 700)
	{
		canvas->drawMonochromeIcon(battery2, 120, 0, 8, 12, 1, TFT_WHITE);
	}
	else if(batteryService->getVoltage() < 700)
	{
		canvas->drawMonochromeIcon(battery3, 120, 0, 8, 12, 1, TFT_WHITE);
	}

}
