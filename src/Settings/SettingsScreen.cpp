#include "SettingsScreen.h"
#include "../UserHWTest/UserHWTest.h"
#include <Input/Input.h>
#include <FS/CompressedFile.h>
#include <Settings.h>
#include <SPIFFS.h>
#include <Pins.hpp>
#include <ByteBoi.h>
#include <Loop/LoopManager.h>

SettingsScreen::SettingsScreen* SettingsScreen::SettingsScreen::instance = nullptr;

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)){

	shutDownSlider = new DiscreteSlider(screenLayout, "Auto shutdown", { 0, 1, 5, 15, 30 }, {});
	volumeSlider = new SliderElement(screenLayout, "Volume", [](int value){
		Settings.get().volume = value;
		Playback.updateGain();
		Playback.tone(500, 100);
	});

	const auto version = ByteBoi.getVer();
	if(version != ByteBoiImpl::v2_0){
		enableLED = new BooleanElement(screenLayout, "LED enable", [](int value){
			Settings.get().RGBenable = value;
			if(!value){
				LED.setRGB(OFF);
			}else{
				LED.setRGB(LEDColor::WHITE);
				instance->blinkTime = millis();
				LoopManager::addListener(instance);
			}
		});
		enableLED->setBooleanSwitch(Settings.get().RGBenable);
		inputTest = new TextElement(screenLayout, "Hardware test", [](int value){
			Context* hwTest = new UserHWTest(*ByteBoi.getDisplay());
			hwTest->push(instance);
			instance->draw();
			instance->screen.commit();
		});
	}else{
		enableLED = nullptr;
		inputTest = nullptr;
	}

	save = new TextElement(screenLayout, "Save", [](int value){
		instance->exitAndSave();
	});

	elements.fill(nullptr);

	if(version == ByteBoiImpl::v2_0){
		NumElements = 3;
		elements = { shutDownSlider, volumeSlider, save };
	}else{
		NumElements = 5;
		elements = { shutDownSlider, volumeSlider, enableLED, inputTest, save };
	}


	instance = this;
	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);
	volumeSlider->setSliderValue(Settings.get().volume);
	SettingsScreen::pack();
}

void SettingsScreen::SettingsScreen::start(){
	Input::getInstance()->addListener(this);
	Input::getInstance()->setButtonHeldRepeatCallback(BTN_RIGHT, 200, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;

		instance->volumeSlider->right();

		instance->draw();
		instance->screen.commit();
	});

	Input::getInstance()->setButtonHeldRepeatCallback(BTN_LEFT, 200, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;

		instance->volumeSlider->left();

		instance->draw();
		instance->screen.commit();
	});

	draw();
	screen.commit();

}

void SettingsScreen::SettingsScreen::stop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_RIGHT);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_LEFT);
	LED.setRGB(LEDColor::OFF);
}

void SettingsScreen::SettingsScreen::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 120, 1);
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 110);
	screen.getSprite()->print("Version 2.0");

	for(int i = 0; i < NumElements; i++){
		if(!reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	for(int i = 0; i < NumElements; i++){
		if(reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
}

void SettingsScreen::SettingsScreen::deinit(){
	free(backgroundBuffer);
}

void SettingsScreen::SettingsScreen::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 120 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("SettingsScreen background unpack error");
		return;
	}

	fs::File bgFile = CompressedFile::open(SPIFFS.open("/launcher/Setts/SettsBg.raw.hs"), 9, 8);
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 120 * 2);
	bgFile.close();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){
	instance = nullptr;
}

void SettingsScreen::SettingsScreen::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->setGutter(5);

	for(uint8_t i = 0; i < NumElements; i++){
		screenLayout->addChild(elements[i]);
	}

	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
}

void SettingsScreen::SettingsScreen::selectApp(int8_t num){

}

void SettingsScreen::SettingsScreen::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			elements[selectedSetting]->left();
			break;

		case BTN_RIGHT:
			elements[selectedSetting]->right();

			break;

		case BTN_UP:
			elements[selectedSetting]->setIsSelected(false);

			selectedSetting--;
			if(selectedSetting < 0){
				selectedSetting = 4;
			}

			elements[selectedSetting]->setIsSelected(true);

			break;

		case BTN_DOWN:
			elements[selectedSetting]->setIsSelected(false);


			selectedSetting++;
			if(selectedSetting > 4){
				selectedSetting = 0;
			}

			elements[selectedSetting]->setIsSelected(true);

			break;

		case BTN_A:
			elements[selectedSetting]->click();
			break;

		case BTN_B:
		case BTN_C:
			exitAndSave();
			break;
	}

	draw();
	screen.commit();

}

void SettingsScreen::SettingsScreen::loop(uint micros){
	if(blinkTime != 0 && millis() - blinkTime >= 200){
		blinkTime = 0;
		LED.setRGB(LEDColor::OFF);
		LoopManager::removeListener(this);
	}
}

void SettingsScreen::SettingsScreen::exitAndSave(){
	Settings.get().shutdownTime = shutDownSlider->getIndex();
	Settings.get().volume = volumeSlider->getSliderValue();
	if(enableLED){
		Settings.get().RGBenable = enableLED->getBooleanSwitch();
	}
	Settings.store();
	Playback.updateGain();
	this->pop();
}
