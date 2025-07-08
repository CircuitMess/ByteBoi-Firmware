#include "TextElement.h"


SettingsScreen::TextElement::TextElement(ElementContainer* parent, String name, std::function<void(int)> onChange) : SettingsElement(parent, name, onChange){

}

void SettingsScreen::TextElement::drawControl(){

}

void SettingsScreen::TextElement::click(){
	onChange(0);
}

void SettingsScreen::TextElement::left(){

}

void SettingsScreen::TextElement::right(){

}
