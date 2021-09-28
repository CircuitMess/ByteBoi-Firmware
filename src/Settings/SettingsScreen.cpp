#include "SettingsScreen.h"
#include <Input/Input.h>
#include <FS/CompressedFile.h>
#include <Settings.h>
#include <SPIFFS.h>
#include <Pins.hpp>

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)),
																   shutDownSlider(new DiscreteSlider(screenLayout, "Auto shutdown", {0, 1, 5, 15, 30})),
																   volumeSlider(new SliderElement(screenLayout, "Volume")),
																   enableLED(new BooleanElement(screenLayout,"LED enable")),
																   inputTest(new TextElement(screenLayout, "Hardware test")),
																   save(new TextElement(screenLayout, "Save")){

	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);
	SettingsScreen::pack();
}

void SettingsScreen::SettingsScreen::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void SettingsScreen::SettingsScreen::stop(){
	Input::getInstance()->removeListener(this);
}

void SettingsScreen::SettingsScreen::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 120, 1);
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 110);
	screen.getSprite()->print("Version 1.0.1");

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

	fs::File bgFile = CompressedFile::open(SPIFFS.open("/launcher/SettingsBg.raw.hs"), 9, 8);
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 120 * 2);
	bgFile.close();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){

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
			}else if(selectedSetting == 2){
				enableLED->toggle();
			}
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selectedSetting == 0){
				shutDownSlider->selectNext();
			}else if(selectedSetting == 1){
				volumeSlider->moveSliderValue(1);
			}else if(selectedSetting == 2){
				enableLED->toggle();
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
			if(selectedSetting == 2){
				enableLED->toggle();
			}
		case BTN_B:
		case BTN_C:
			Settings.get().shutdownTime = shutDownSlider->getIndex();
			Settings.store();
			this->pop();
			draw();
			screen.commit();
			break;
	}

}
