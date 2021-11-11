#include <Arduino.h>
#include <CircuitOS.h>
#include <ByteBoi.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include "src/Launcher.h"
#include "src/GameManagement/GameManager.h"
#include "src/IntroScreen.h"
#include <SleepService.h>
#include "src/UserHWTest/UserHWTest.h"


void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	Battery.disableShutdown(true);
	Sleep.begin();
	ByteBoi.unbindMenu();

	if(!Settings.get().hwTested){
		UserHWTest* test = new UserHWTest(*ByteBoi.getDisplay());
		test->setDoneCallback([](UserHWTest* test){
			Settings.get().hwTested = true;
			Settings.store();

			ESP.restart();
		});

		test->unpack();
		test->start();

		return;
	}

	Launcher* launcher = new Launcher(ByteBoi.getDisplay());

	Loader.checkLoaded();

	LoopManager::addListener(&Games);
	Games.detectSD();
	Games.resetGamesRescanned();

	Context* intro = new IntroScreen(*ByteBoi.getDisplay(), launcher);
	intro->unpack();
	intro->start();
}

void loop(){
	LoopManager::loop();
}
