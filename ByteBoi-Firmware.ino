#include <Arduino.h>
#include <CircuitOS.h>

#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Input/InputI2C.h>
#include <Input/I2cExpander.h>
#include <Audio/Piezo.h>
#include <WiFi.h>
#include <ByteBoi.h>
#include <SPIFFS.h>
#include "src/Launcher.h"
#include <Settings.h>
#include <Util/Settings.h>
#include <SPIFFS.h>
#include "src/GameManagement/GameManager.h"
#include "src/BatteryPopup/BatteryPopupService.h"

Launcher* launcher;

Display* display;

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	ByteBoi.unbindMenu();
	Games.scanGames();
	display = ByteBoi.getDisplay();

	BatteryPopup.setTFT(display->getTft());
	LoopManager::addListener(&BatteryPopup);
	Input::getInstance()->addListener(&BatteryPopup);
	Battery.disableShutdown(true);



	LoopManager::addListener(Input::getInstance());
	launcher = new Launcher(display);

	runningContext = launcher;


	launcher->unpack();

	launcher->start();

}

void loop(){
	LoopManager::loop();
}
