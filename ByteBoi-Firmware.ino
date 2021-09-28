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
#include <Battery/BatteryPopupService.h>

Launcher* launcher;

Display* display;

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	ByteBoi.unbindMenu();
	BatteryPopup.enablePopups(true);

	Games.scanGames();
	LoopManager::addListener(&Games);

	Context* launcher = new Launcher(ByteBoi.getDisplay());
	launcher->unpack();
	launcher->start();
}

void loop(){
	LoopManager::loop();
}
