#include "BooleanElement.h"

SettingsScreen::BooleanElement::BooleanElement(ElementContainer* partent, String name, std::function<void(int)> onChange) : SettingsElement(partent, name, onChange){

}

void SettingsScreen::BooleanElement::click(){
	booleanSwitch = !booleanSwitch;
	onChange(booleanSwitch);
}

void SettingsScreen::BooleanElement::left(){
	booleanSwitch = !booleanSwitch;
	onChange(booleanSwitch);

}

void SettingsScreen::BooleanElement::right(){
	booleanSwitch = !booleanSwitch;
	onChange(booleanSwitch);

}

void SettingsScreen::BooleanElement::drawControl(){
	getSprite()->setTextColor(TFT_BLACK);
	getSprite()->setTextSize(1);
	getSprite()->setTextFont(1);
	getSprite()->setCursor(getTotalX() + 122, getTotalY() + 7);

	if(booleanSwitch){
		getSprite()->fillRoundRect(getTotalX() + 120, getTotalY() + 5, 27, 11, 2, TFT_GREEN);
		getSprite()->println("ON");
	}else if(!booleanSwitch){
		getSprite()->fillRoundRect(getTotalX() + 120, getTotalY() + 5, 27, 11, 2, TFT_DARKGREY);
		getSprite()->println("OFF");
	}
	getSprite()->drawRoundRect(getTotalX() + 120, getTotalY() + 5, 27, 11, 2, TFT_BLACK);

}

bool SettingsScreen::BooleanElement::getBooleanSwitch() const{
	return booleanSwitch;
}

void SettingsScreen::BooleanElement::setBooleanSwitch(bool booleanSwitch){
	BooleanElement::booleanSwitch = booleanSwitch;
}
