#include <Arduino.h>
#include <CircuitOS.h>
#include <ByteBoi.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include "src/Launcher.h"
#include "src/GameManagement/GameManager.h"
#include "src/IntroScreen.h"
#include <SleepService.h>
#include "src/JigHWTest/JigHWTest.h"
#include "src/UserHWTest/UserHWTest.h"


bool checkJig(){
#define JIG_A 12
#define JIG_B 14

	pinMode(JIG_A, OUTPUT);
	pinMode(JIG_B, INPUT_PULLDOWN);

	digitalWrite(JIG_A, HIGH);
	delay(10);
	if(digitalRead(JIG_B) != HIGH) return false;

	digitalWrite(JIG_A, LOW);
	delay(10);
	if(digitalRead(JIG_B) != LOW) return false;

	return true;
}

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	Sleep.begin();
	ByteBoi.unbindMenu();

	if(checkJig()){
		JigHWTest* test = new JigHWTest(*ByteBoi.getDisplay());
		test->start();

		for(;;);
	}

	ByteBoi.checkSD();
	Games.detectSD();
	Games.resetGamesRescanned();
	Loader.checkLoaded();
	LoopManager::addListener(&Games);

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

	Context* intro = new IntroScreen(*ByteBoi.getDisplay(), launcher);
	intro->unpack();
	intro->start();
}

void loop(){
	LoopManager::loop();
}
