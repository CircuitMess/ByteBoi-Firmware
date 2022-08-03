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
SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)),
																   shutDownSlider(new DiscreteSlider(screenLayout, "Auto shutdown", {0, 1, 5, 15, 30})),
																   volumeSlider(new SliderElement(screenLayout, "Volume")),
																   enableLED(new BooleanElement(screenLayout, "LED enable")),
																   inputTest(new TextElement(screenLayout, "Hardware test")),
																   save(new TextElement(screenLayout, "Save")){
	instance = this;
	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);
	volumeSlider->setSliderValue(Settings.get().volume);
	enableLED->setBooleanSwitch(Settings.get().RGBenable);
	SettingsScreen::pack();
}

void SettingsScreen::SettingsScreen::start(){
	Input::getInstance()->addListener(this);
	Input::getInstance()->setButtonHeldRepeatCallback(BTN_RIGHT, 200, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;
		instance->volumeSlider->moveSliderValue(1);

		Settings.get().volume = instance->volumeSlider->getSliderValue();
		Playback.updateGain();
		Playback.tone(500, 50);

		instance->draw();
		instance->screen.commit();
	});

	Input::getInstance()->setButtonHeldRepeatCallback(BTN_LEFT, 200, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;
		instance->volumeSlider->moveSliderValue(-1);

		Settings.get().volume = instance->volumeSlider->getSliderValue();
		Playback.updateGain();
		Playback.tone(500, 50);

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
	screen.getSprite()->print("Version 1.1");

	for(int i = 0; i < 5; i++){
		if(!reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	for(int i = 0; i < 5; i++){
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
	screenLayout->addChild(shutDownSlider);
	screenLayout->addChild(volumeSlider);
	screenLayout->addChild(enableLED);
	screenLayout->addChild(inputTest);
	screenLayout->addChild(save);

	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
}

void SettingsScreen::SettingsScreen::selectApp(int8_t num){

}

void SettingsScreen::SettingsScreen::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(selectedSetting == 0){
				shutDownSlider->selectPrev();
			}else if(selectedSetting == 1){
				volumeSlider->moveSliderValue(-1);
				Settings.get().volume = volumeSlider->getSliderValue();
				Playback.updateGain();
				Playback.tone(500, 200);
			}else if(selectedSetting == 2){
				enableLED->toggle();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				if(!Settings.get().RGBenable){
					LED.setRGB(OFF);
				}else{
					LED.setRGB(LEDColor::WHITE);
					instance->blinkTime = millis();
					LoopManager::addListener(this);
				}
			}
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selectedSetting == 0){
				shutDownSlider->selectNext();
			}else if(selectedSetting == 1){
				volumeSlider->moveSliderValue(1);
				Settings.get().volume = volumeSlider->getSliderValue();
				Playback.updateGain();
				Playback.tone(500, 200);
			}else if(selectedSetting == 2){
				enableLED->toggle();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				if(!Settings.get().RGBenable){
					LED.setRGB(OFF);
				}else{
					LED.setRGB(LEDColor::WHITE);
					instance->blinkTime = millis();
					LoopManager::addListener(this);
				}
			}
			draw();
			screen.commit();
			break;

		case BTN_UP:
			selectedSetting--;
			if(selectedSetting < 0){
				selectedSetting = 4;
			}
			if(selectedSetting == 0){
				shutDownSlider->setIsSelected(true);
			}else{
				shutDownSlider->setIsSelected(false);
			}
			if(selectedSetting == 1){
				volumeSlider->setIsSelected(true);
			}else{
				volumeSlider->setIsSelected(false);
			}
			if(selectedSetting == 2){
				enableLED->setIsSelected(true);
			}else{
				enableLED->setIsSelected(false);
			}
			if(selectedSetting == 3){
				inputTest->setIsSelected(true);
			}else{
				inputTest->setIsSelected(false);
			}
			if(selectedSetting == 4){
				save->setIsSelected(true);
			}else{
				save->setIsSelected(false);
			}

			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			selectedSetting++;
			if(selectedSetting > 4){
				selectedSetting = 0;
			}
			if(selectedSetting == 0){
				shutDownSlider->setIsSelected(true);
			}else{
				shutDownSlider->setIsSelected(false);
			}
			if(selectedSetting == 1){
				volumeSlider->setIsSelected(true);
			}else{
				volumeSlider->setIsSelected(false);
			}
			if(selectedSetting == 2){
				enableLED->setIsSelected(true);
			}else{
				enableLED->setIsSelected(false);
			}
			if(selectedSetting == 3){
				inputTest->setIsSelected(true);
			}else{
				inputTest->setIsSelected(false);
			}
			if(selectedSetting == 4){
				save->setIsSelected(true);
			}else{
				save->setIsSelected(false);
			}
			draw();
			screen.commit();
			break;

		case BTN_A:
			if(selectedSetting == 1){
				if(volumeSlider->getSliderValue() == 0){
					volumeSlider->setSliderValue(180);
				}else{
					volumeSlider->setSliderValue(0);
				}
				Settings.get().volume = instance->volumeSlider->getSliderValue();
				Playback.updateGain();
				Playback.tone(500, 50);
			}else if(selectedSetting == 2){
				enableLED->toggle();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				if(!Settings.get().RGBenable){
					LED.setRGB(OFF);
				}else{
					LED.setRGB(LEDColor::WHITE);
					instance->blinkTime = millis();
					LoopManager::addListener(this);
				}
			}else if(selectedSetting == 3){
				Context* hwTest = new UserHWTest(*ByteBoi.getDisplay());
				hwTest->push(this);
				draw();
				screen.commit();
				break;
			}else if(selectedSetting == 4){
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.get().volume = volumeSlider->getSliderValue();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				Settings.store();
				Playback.updateGain();
				this->pop();
			}
			draw();
			screen.commit();
			break;
		case BTN_B:
		case BTN_C:
			Settings.get().shutdownTime = shutDownSlider->getIndex();
			Settings.get().volume = volumeSlider->getSliderValue();
			Settings.get().RGBenable = enableLED->getBooleanSwitch();
			Settings.store();
			Playback.updateGain();
			this->pop();
			draw();
			screen.commit();
			break;
	}

}

void SettingsScreen::SettingsScreen::loop(uint micros){
	if(blinkTime != 0 && millis() - blinkTime >= 200){
		blinkTime = 0;
		LED.setRGB(LEDColor::OFF);
		LoopManager::removeListener(this);
	}
}
