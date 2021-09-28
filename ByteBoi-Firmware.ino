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
#include "src/IntroScreen.h"

Launcher* launcher;

Display* display;

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	ByteBoi.unbindMenu();
	BatteryPopup.enablePopups(true);

	Games.scanGames();
	LoopManager::addListener(&Games);

	Context* intro = new IntroScreen(*ByteBoi.getDisplay());
	intro->unpack();
	intro->start();
}

void loop(){
	LoopManager::loop();
}
