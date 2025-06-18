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
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	int c;
	while(millis() - start < 500){
		vTaskDelay(1);
		c = getchar();
		if(c == EOF) continue;
		buf[wp] = (char) c;
		wp = (wp + 1) % 7;

		for(int i = 0; i < 7; i++){
			int match = 0;
			static const char* target = "JIGTEST";

			for(int j = 0; j < 7; j++){
				match += buf[(i + j) % 7] == target[j];
			}

			if(match == 7) return true;
		}
	}

	return false;
}

void setup(){
	Serial.begin(115200);

	if(checkJig()){
		ByteBoi.initVer(1);
		ByteBoi.begin();
		ByteBoi.unbindMenu();

		JigHWTest* test = new JigHWTest(*ByteBoi.getDisplay());
		test->start();

		for(;;);
	}else{
		printf("Hello\n");
	}

	ByteBoi.begin();
	Sleep.begin();
	ByteBoi.unbindMenu();

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
