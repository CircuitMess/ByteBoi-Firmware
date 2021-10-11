#include <Arduino.h>
#include <CircuitOS.h>
#include <ByteBoi.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Battery/BatteryPopupService.h>
#include "src/Launcher.h"
#include "src/GameManagement/GameManager.h"
#include "src/IntroScreen.h"
#include "src/JigHWTest/JigHWTest.h"

void setup(){
	Serial.begin(115200);
	ByteBoi.begin();
	ByteBoi.unbindMenu();
	BatteryPopup.enablePopups(true);


	JigHWTest test(*ByteBoi.getDisplay());
	test.start();

	Games.scanGames();
	LoopManager::addListener(&Games);

	Launcher* launcher = new Launcher(ByteBoi.getDisplay());

	Context* intro = new IntroScreen(*ByteBoi.getDisplay(), launcher);
	intro->unpack();
	intro->start();
}

void loop(){
	LoopManager::loop();
}
