#include "NumericSettingElement.h"
#include <Arduino.h>
#include <string>
#include <Settings.h>

SettingsMenu::NumericSettingElement::NumericSettingElement(ElementContainer* parent, Setting* setting) :
		SettingElement(parent, setting)
{
	values = ((NumericSettingParams*)setting->params)->values;
	numSteps = values.size();
	currentStep = 0;
	if(setting->storeLocation != nullptr && values.indexOf(*((int*)setting->storeLocation)) != 0xFFFFFFFF)
	{
		currentStep = max((uint)0, values.indexOf(*((int*)setting->storeLocation)));
	}
	blinkTime = 0;
	active = 0;
}
void SettingsMenu::NumericSettingElement::draw()
{
	getSprite()->setCursor(getTotalX() + 2, getTotalY() - 1);
	getSprite()->setTextColor(TFT_WHITE);
	if(active)
	{
		getSprite()->setTextColor(TFT_YELLOW);
	}
	getSprite()->setTextSize(1);
	getSprite()->setFont(&fonts::Font2);
	getSprite()->setTextWrap(0);
	getSprite()->print(setting->label.c_str());
	getSprite()->print(": ");
	getSprite()->setTextColor(TFT_WHITE);
	getSprite()->print(parseSeconds(values[currentStep]).c_str());
	if(blinkToggle)
	{
		getSprite()->setCursor(getTotalX() + 2, getTotalY() + 15);
		getSprite()->print(parseSeconds(values[0]).c_str());
		getSprite()->setCursor(getTotalX() + getWidth() - 25, getTotalY() + 15);
		getSprite()->print(parseSeconds(values[values.size() - 1]).c_str());
	}

	getSprite()->drawRoundRect(getTotalX() + 28, getTotalY() + 19, getWidth() - 56, 7, 2, TFT_WHITE);
	getSprite()->fillRect(getTotalX() + 30, getTotalY() + 21, map(currentStep, 0, numSteps - 1, 0, getWidth() - 60), 3, TFT_WHITE);
	Element::draw();
}
String SettingsMenu::NumericSettingElement::parseSeconds(uint seconds)
{
	String str1 = "";
	if(seconds >= 3600)
	{
		str1 += String(uint(seconds/3600)) + "h";
	}
	else if(seconds >= 60)
	{
		str1 + String(uint(seconds/60)) + "min";
	}
	else
	{
		str1 + String(seconds) + "s";
	}
	return str1;
}
void SettingsMenu::NumericSettingElement::pressLeft()
{
	currentStep = max(0, currentStep - 1);
	*((int*)setting->storeLocation) = values[currentStep];
}
void SettingsMenu::NumericSettingElement::pressRight()
{
	currentStep = min(numSteps - 1, currentStep + 1);
	*((int*)setting->storeLocation) = values[currentStep];
}
void SettingsMenu::NumericSettingElement::pressSelect()
{
}