#include "SliderElement.h"

SettingsScreen::SliderElement::SliderElement(ElementContainer* partent, String name) : SettingsElement(partent, name){

}

void SettingsScreen::SliderElement::toggle(){
	sliderIsSelected = !sliderIsSelected;
}

void SettingsScreen::SliderElement::moveSliderValue(int8_t value){
	sliderValue = constrain(sliderValue + 10*value, 0, 255);
}

void SettingsScreen::SliderElement::setSliderValue(uint8_t sliderValue){
	SliderElement::sliderValue = sliderValue;
}

uint8_t SettingsScreen::SliderElement::getSliderValue() const{
	return sliderValue;
}

void SettingsScreen::SliderElement::drawControl(){
	float movingCursor;
	if(sliderValue == 0){
		movingCursor = 0;
	}else{
		movingCursor = ((float) sliderValue / 255) * 51.0f;
	}
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 10, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 153, getTotalY() + 10, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 12, 55, 1, TFT_WHITE);
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 9, 4, 7, 1, TFT_WHITE);

}
