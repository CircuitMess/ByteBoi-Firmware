#include "SettingsMenu.h"
#include <ByteBoi.h>
#include "Elements/NumericSettingElement.h"
#include "Elements/BoolSettingElement.h"
#include <Audio/Piezo.h>
#include <Settings.h>

Vector<SettingsMenu::Setting> settingsVector = {
	SettingsMenu::Setting(SettingsMenu::Setting::Type::NUMERIC, new SettingsMenu::NumericSettingParams(Vector<int>{0, 300, 600, 1800, 3600}), std::string("Shutdown"), nullptr),
	SettingsMenu::Setting(SettingsMenu::Setting::Type::BOOLEAN, nullptr, std::string("Audio"), nullptr)
};

SettingsMenu::SettingsMenu* SettingsMenu::SettingsMenu::instance = nullptr;
SettingsMenu::SettingsMenu::SettingsMenu(Display& display) :
		Context(display), display(&display), canvas(screen.getSprite()),
		layout(new LinearLayout(&screen, VERTICAL))
{
	instance = this;

	settingsVector[0].storeLocation = &(Settings.get().shutdownTime);
	settingsVector[1].storeLocation = &(Settings.get().mute);

	layout->setWHType(PARENT, PARENT);
	layout->setPadding(3);
	layout->setGutter(7);
	layout->reflow();
	screen.addChild(layout);
	for (uint8_t i = 0; i < settingsVector.size(); i++)
	{
		SettingElement* element;
		switch (settingsVector[i].type)
		{
		case Setting::Type::NUMERIC:
			element = new NumericSettingElement(layout, &settingsVector[i]);
			break;
		case Setting::Type::BOOLEAN:
			element = new BoolSettingElement(layout, &settingsVector[i]);
			break;
		default:
			break;
		}
		elements.push_back(element);
		layout->addChild(element);
	}
	layout->reposChildren();
	cursor = 0;
	elements[0]->setActive(1);
}
void SettingsMenu::SettingsMenu::draw()
{
	canvas->clear(TFT_DARKGREY);
	screen.draw();
}
void SettingsMenu::SettingsMenu::loop(uint _time)
{
	for (uint8_t i = 0; i < elements.size(); i++)
	{
		if(elements[i]->isActive())
		{
			elements[i]->update(_time);
		}
	}
	draw();
	screen.commit();
}
void SettingsMenu::SettingsMenu::start()
{
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		Settings.store();
		Piezo.setMute(!Settings.get().volume);
		instance->pop();
	});
	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance->cursor > 0)
		{
			instance->elements[instance->cursor]->setActive(0);
			instance->cursor--;
			instance->elements[instance->cursor]->setActive(1);
			instance->setElementCallbacks();
			Piezo.tone(150, 50);
		}
	});
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance->cursor < settingsVector.size() - 1)
		{
			instance->elements[instance->cursor]->setActive(0);
			instance->cursor++;
			instance->elements[instance->cursor]->setActive(1);
			instance->setElementCallbacks();
			Piezo.tone(150, 50);
		}
	});
	setElementCallbacks();
	LoopManager::addListener(this);
}
void SettingsMenu::SettingsMenu::stop()
{
	LoopManager::removeListener(this);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
}
void SettingsMenu::SettingsMenu::pack()
{
}
void SettingsMenu::SettingsMenu::setElementCallbacks()
{
	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		instance->elements[instance->cursor]->pressLeft();
		Piezo.tone(150, 50);
	});
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		instance->elements[instance->cursor]->pressRight();
		Piezo.tone(150, 50);
	});
}