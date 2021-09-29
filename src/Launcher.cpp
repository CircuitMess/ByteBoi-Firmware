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
#include "DescriptionModal.h"
#include "Settings/SettingsScreen.h"
#include <SD.h>
#include <SPIFFS.h>
#include <FS/CompressedFile.h>

#include <utility>

Launcher* Launcher::instance = nullptr;

LauncherItem::LauncherItem(const GameImage& image, String text, std::function<void()> exec) : image(image), text(std::move(text)), exec(std::move(exec)){}

Launcher::Launcher(Display* display) : Context(*display), display(display), genericIcon(screen.getSprite()), settingsIcon(screen.getSprite()){
	canvas = screen.getSprite();
	scroller = new GameScroller(canvas, items);
	logo = new Logo(canvas);
	title = new GameTitle(canvas);

	instance = this;
	canvas->setChroma(TFT_TRANSPARENT);
	splash = new Splash(scroller);

	fs::File icon = SPIFFS.open("/Launcher/genericGame.raw");
	if(icon){
		icon.read(reinterpret_cast<uint8_t*>(genericIcon.getBuffer()), 64 * 64 * 2);
	}else{
		genericIcon = GameImage();
	}
	icon.close();

	fs::File settIcon = SPIFFS.open("/launcher/SettingsIcon.raw");
	if(settIcon){
		settIcon.read(reinterpret_cast<uint8_t*>(settingsIcon.getBuffer()), 64 * 64 * 2);
	}else{
		settingsIcon = GameImage();
	}
	settIcon.close();

	Games.setGameListener(this);
	load();
	title->change("");
	scroller->splash(0);

	Launcher::pack();
}

Launcher::~Launcher(){
	Games.setGameListener(nullptr);
}

void Launcher::load(){
	// TODO: check if the copying calls the GameImage copy constructor. otherwise multiple GameInfo will attempt to destruct the same buffer
	// TODO: also, add "no ownership of buffer" mode or something similar to GameImage to avoid multiple buffers for genericIcon
	items.clear();

	if(!Games.SDinserted()){
		items.emplace_back(GameImage(), "No SD card", [](){}); // TODO: icon
	}else if(Games.getGames().empty()){
		items.emplace_back(GameImage(), "SD card empty", [](){}); // TODO: icon
	}else{
		for(const auto& game : Games.getGames()){
			items.emplace_back(GameImage(), game->name.c_str(), [game](){
				GameLoader::loadGame(game);
			});

			LauncherItem& item = items.back();
			item.text = game->name.c_str();

			fs::File icon = SD.open(game->icon.c_str());
			if(icon){
				item.image = GameImage(canvas);
				if(icon.read(reinterpret_cast<uint8_t*>(item.image.getBuffer()), 64 * 64 * 2) != 64 * 64 * 2){
					item.image = GameImage();
				}
				icon.close();
			}

			if(!item.image){
				item.image = genericIcon;
			}
		}
	}

	items.emplace_back(settingsIcon, "Settings", [this](){
		Display& display = *this->getScreen().getDisplay();
		SettingsScreen::SettingsScreen* settingsScreen =new SettingsScreen::SettingsScreen(display);
		settingsScreen->push(this);
	});

	if(!items.empty() && items.size() < 4){ // scroller expects at least 4 items
		if(items.size() == 1){ // if only one element, duplicate it 3 times
			for(int i = 0; i < 3; i++){
				items.emplace_back(items.front());
			}
		}else{ // for 2 and 3 elements, duplicate them so we get to at least 4
			int count = items.size();
			for(int i = 0; i < count; i++){
				items.emplace_back(items[i]);
			}
		}
	}

	scroller->reset();
	selectedGame = 0;
	title->change(items[selectedGame].text);
}

void Launcher::start(){
	if(splash == nullptr){
		bindInput();
	}else{
		title->change(items[selectedGame].text);
	}

	draw();
	screen.commit();
	LoopManager::addListener(this);

	LoopManager::addListener(logo);
}

void Launcher::stop(){
	LoopManager::removeListener(this);
	LoopManager::removeListener(logo);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
}

void Launcher::prev(){
	uint8_t selecting = instance->scroller->prev();
	if(selecting != selectedGame){
		instance->title->change(items[selecting].text);
	}
	selectedGame = selecting;
}

void Launcher::next(){
	uint8_t selecting = instance->scroller->next();
	if(selecting != selectedGame){
		instance->title->change(items[selecting].text);
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
		instance->items[instance->selectedGame].exec();
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;
		// TODO: check for non-games
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
		}
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
	load();
	if(splash != nullptr){
		title->change("");
	}
}
