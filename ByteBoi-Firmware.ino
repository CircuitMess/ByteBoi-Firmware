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

Launcher* launcher;

Display* display;

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	ByteBoi.unbindMenu();
	Games.scanGames();

	pinMode(36, INPUT);
	pinMode(34, INPUT);

	display = ByteBoi.getDisplay();


	LoopManager::addListener(Input::getInstance());
	LoopManager::addListener(&Games);
	launcher = new Launcher(display);

	launcher->unpack();
	launcher->start();
}

void loop(){
	LoopManager::loop();
}
