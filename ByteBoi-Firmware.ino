#include <Arduino.h>
#include <CircuitOS.h>

#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Input/Input.h>
#include <Input/InputI2C.h>
#include <Input/I2cExpander.h>
#include <Audio/Piezo.h>
#include <WiFi.h>
#include "src/Pins.hpp"

#include "src/Launcher.h"
#include "src/Services/BatteryService.h"
#include "src/Services/SleepService.h"
#include "src/SettingsMenu/SettingsStruct.hpp"

Launcher* launcher;
BatteryService* batteryService;
SleepService* sleepService;

Display* display;

void setup(){
	Serial.begin(115200);
	Serial.println();

	if(psramFound()){
		Serial.printf("PSRAM init: %s, free: %d B\n", psramInit() ? "Yes" : "No", ESP.getFreePsram());
	}else{
		Serial.println("No PSRAM detected");
	}

	display = new Display(160, 120, -1, 3);
	I2cExpander* expander = new I2cExpander();

	display->begin();
	display->getBaseSprite()->clear(TFT_RED);
	display->commit();
	expander->begin(0x74, 14, 33);
	expander->pinMode(BL_PIN, OUTPUT);
	expander->pinWrite(BL_PIN, 0);

	Input* input = new InputI2C(expander);
	input->preregisterButtons({ BTN_A, BTN_B, BTN_C, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT });
	Piezo.begin(BUZZ_PIN);


	WiFi.mode(WIFI_OFF);
	btStop();

	Piezo.begin(BUZZ_PIN);

	if(!Settings::init(new SettingsStruct, sizeof(SettingsStruct))){
		settings()->shutdownTime = 300; //5 minutes
		settings()->sleepTime = 30; //30 seconds
		settings()->audio = true; //audio on
		settings()->calibrated = false;
		Settings::store();
	}

#ifdef DEBUG_FLAG
/*	LoopManager::addListener(new SerialID);

	for(uint8_t i = 0; i < 7; i++)
	{
		ByteBoi.getExpander()->pinMode(i, INPUT_PULLUP);
	}
	uint8_t portRead = ByteBoi.getExpander()->portRead() & 0b01111111;

	if(!portRead && !settings()->calibrated)
	{
		HardwareTest test(*ByteBoi.getDisplay());
		test.start();
	}*/

#endif

	Piezo.setMute(!settings()->audio);

	batteryService = new BatteryService(*display);
	sleepService = new SleepService(*display);

//	LoopManager::addListener(batteryService);
	LoopManager::addListener(Input::getInstance());

	launcher = new Launcher(display, batteryService);
	runningContext = launcher;

	launcher->unpack();
	launcher->start();
	sleepService->start();
//	LoopManager::addListener(sleepService);
}

void loop(){
	LoopManager::loop();
}
