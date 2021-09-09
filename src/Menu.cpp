#include "Menu.h"
#include <ByteBoi.h>
#include <Display/Sprite.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Audio/Piezo.h>
#include "SettingsMenu/SettingsStruct.hpp"
#include "Services/BatteryService.h"
#include <SPIFFS.h>
#include <FS/CompressedFile.h>

Menu* Menu::instance = nullptr;

Menu::Menu(Context* currentContext) : Modal(*currentContext, 130, 57), canvas(screen.getSprite()),
	layout(&screen, VERTICAL), audioLayout(&layout, HORIZONTAL), exit(&layout, 120, 20),
	muteText(&audioLayout, 50, 20), audioSwitch(&audioLayout){
	instance = this;

	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/background.raw.hs"), 13, 12);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 120 * 2);
	backgroundFile.close();

	buildUI();
	audioSwitch.set(settings()->audio, true);
}
Menu::~Menu(){
	free(backgroundBuffer);

}

void Menu::start(){
	selectElement(0);
	bindInput();
	audioSwitch.set(settings()->audio, true);
	LoopManager::addListener(this);
}

void Menu::stop(){
	Settings::store();
	releaseInput();
	LoopManager::removeListener(this);

}

void Menu::bindInput(){

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			instance->audioSwitch.toggle();
			settings()->audio = instance->audioSwitch.getState();
			Piezo.setMute(!settings()->audio);
			Piezo.tone(500, 50);
		}else{
			instance->pop();
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance == nullptr) return;
		instance->selectElement(instance->selectedElement == 0 ? 1 : 0);
		Piezo.tone(500, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance == nullptr) return;
		instance->selectElement((instance->selectedElement + 1) % 2);
		Piezo.tone(500, 50);
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			settings()->audio = 1;
			Piezo.setMute(!settings()->audio);
			if(instance->audioSwitch.getState() == false)
			{
				Piezo.tone(500, 50);
			}
			instance->audioSwitch.set(true);
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		if(instance->selectedElement == 0){
			instance->audioSwitch.set(false);
			settings()->audio = 0;
			Piezo.setMute(!settings()->audio);
			Piezo.tone(500, 50);
		}
	});
}

void Menu::releaseInput(){
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}

void Menu::selectElement(uint8_t index){
	layout.reposChildren();
	audioLayout.reposChildren();
	selectedElement = index;

	exit.setColor(TFT_WHITE);
	muteText.setColor(TFT_WHITE);

	TextElement* selectedText = selectedElement == 0 ? &muteText : &exit;
	selectAccum = 0;
	selectedX = selectedText->getX();
	selectedText->setColor(TFT_YELLOW);
}

void Menu::draw(){

	canvas->fillRect(0, 0, 130, 57, TFT_DARKGREY);
	canvas->drawRect(0, 0, 130, 57, TFT_LIGHTGREY);
	canvas->drawRect(1, 1, 130 - 2, 57 - 2, TFT_LIGHTGREY);
	canvas->drawIcon(backgroundBuffer, 0, 0, 160, 120, 1);
	screen.draw();

}

void Menu::loop(uint micros){

	selectAccum += (float) micros / 1000000.0f;
	TextElement* selectedText = selectedElement == 0 ? &muteText : &exit;
	int8_t newX = selectedX + sin(selectAccum * 5.0f) * 3.0f;
	selectedText->setX(newX);
	draw();
	screen.commit();
}

void Menu::buildUI(){
	layout.setWHType(CHILDREN, CHILDREN);
	layout.setPadding(5);
	layout.setGutter(5);
	layout.reflow();

	audioLayout.setWHType(PARENT, CHILDREN);
	audioLayout.addChild(&muteText);
	audioLayout.addChild(&audioSwitch);
	audioLayout.reflow();

	layout.addChild(&audioLayout);
	layout.addChild(&exit);

	layout.reflow();
	screen.addChild(&layout);
	screen.repos();

	muteText.setText("Sound");
	muteText.setFont(1);
	muteText.setSize(1);
	muteText.setColor(TFT_WHITE);

	exit.setText("Exit Game");
	exit.setFont(1);
	exit.setSize(1);
	exit.setColor(TFT_WHITE);
	exit.setAlignment(TextElement::CENTER);
}
