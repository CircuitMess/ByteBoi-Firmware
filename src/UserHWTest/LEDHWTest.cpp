#include <Pins.hpp>
#include <ByteBoi.h>
#include <Loop/LoopManager.h>
#include "LEDHWTest.h"

LEDHWTest::LEDHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){
}

LEDHWTest::~LEDHWTest(){

}

void LEDHWTest::draw(){
	userHwTest->getScreen().getSprite()->setTextFont(1);
	userHwTest->getScreen().getSprite()->setTextSize(1);
	userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("LEDs should flash") / 2, 40);
	userHwTest->getScreen().getSprite()->print("LEDs should flash");
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("red,green, and blue.") / 2, 50);
	userHwTest->getScreen().getSprite()->print("red, green, and blue.");
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("Press A to continue") / 2, 110);
	userHwTest->getScreen().getSprite()->print("Press A to continue.");
}

void LEDHWTest::start(){
	LED.setRGB(OFF);
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void LEDHWTest::stop(){
	LED.setRGB(OFF);
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void LEDHWTest::buttonPressed(uint id){
	switch(id){
		case BTN_A:
		case BTN_B:
		case BTN_C:
		case BTN_UP:
		case BTN_DOWN:
		case BTN_LEFT:
		case BTN_RIGHT:
			userHwTest->currentTestDone();
			break;
	}
}

void LEDHWTest::loop(uint micros){
	if(millis() - previousTime >= 500){
		previousTime = millis();
		if(rgbLED.getRGB() == OFF){
			rgbLED.setRGB(static_cast<LEDColor>(ledArray[index%3]));
			index++;
		}else{
			rgbLED.setRGB(OFF);
		}
	}
}

