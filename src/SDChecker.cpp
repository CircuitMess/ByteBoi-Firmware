#include <ByteBoi.h>
#include "SDChecker.h"
#include <Loop/LoopManager.h>

void SDChecker::start(){
	if(ByteBoi.getExpander()) return;

	checkTime = millis();
	LoopManager::addListener(this);
}


void SDChecker::stop(){
	LoopManager::removeListener(this);
}

void SDChecker::loop(uint micros){
	if(millis() - checkTime < checkInterval) return;
	checkTime = millis();
	check();
}

void SDChecker::check(){
	ByteBoi.checkSD();
}
