#include "DiscreteSlider.h"


SettingsScreen::DiscreteSlider::DiscreteSlider(ElementContainer* parent, String name, std::vector<uint8_t> shutDownTime, std::function<void(int)> onChange) : SettingsElement(parent, name, onChange), shutDownTime(shutDownTime){

}

void SettingsScreen::DiscreteSlider::click(){
	sliderIsSelected = !sliderIsSelected;
}

void SettingsScreen::DiscreteSlider::right(){
	if(shutDownTime.empty() || !sliderIsSelected) return;
	index += 1;
	index = min(index, (int) shutDownTime.size() - 1);
}

void SettingsScreen::DiscreteSlider::left(){
	if(shutDownTime.empty() || !sliderIsSelected) return;
	index -= 1;
	index = max(index, 0);

}

void SettingsScreen::DiscreteSlider::drawControl(){
	long movingCursor = map(index, 0, shutDownTime.size() - 1, 0, 51);

	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 12, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 153, getTotalY() + 12, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 14, 55, 1, TFT_WHITE);
	if(sliderIsSelected){
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 10, 4, 9, 1, TFT_RED);
	}else{
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 11, 4, 7, 1, TFT_WHITE);
	}
	getSprite()->setTextColor(TFT_WHITE);
	if(index == 0){
		getSprite()->setCursor(getTotalX() + 120, getTotalY() + 2);
		getSprite()->println("OFF");
		return;
	}
	getSprite()->setCursor(getTotalX() + 115, getTotalY() + 2);
	getSprite()->println(shutDownTime[index]);
	getSprite()->setCursor(getTotalX() + 127, getTotalY() + 2);
	getSprite()->println("min");

}

bool SettingsScreen::DiscreteSlider::isSliderSelected() const{
	return sliderIsSelected;
}

void SettingsScreen::DiscreteSlider::setIndex(int index){
	DiscreteSlider::index = index;

}

int SettingsScreen::DiscreteSlider::getIndex() const{
	return index;

}


