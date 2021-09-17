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
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
#include <Settings.h>
#include <Util/Settings.h>
#include <SPIFFS.h>

Launcher* launcher;
BatteryService* batteryService;
SleepService* sleepService;

Display* display;

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();


	pinMode(36, INPUT);
	pinMode(34, INPUT);

	display = ByteBoi.getDisplay();

	batteryService = new BatteryService(*display);
	sleepService = new SleepService(*display);

	LoopManager::addListener(batteryService);
	LoopManager::addListener(Input::getInstance());
	launcher = new Launcher(display, batteryService);

	runningContext = launcher;


	launcher->unpack();

	launcher->start();

	sleepService->start();
	LoopManager::addListener(sleepService);

}

void loop(){
	LoopManager::loop();
}
