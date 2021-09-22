#include "BatteryPopupService.h"
#include <ByteBoi.h>
#include <Support/ContextTransition.h>
#include <Support/ModalTransition.h>
#include "ShutdownPopup.h"
#include "WarningPopup.h"
#include <Settings.h>
#include <Display/Display.h>
#include <WiFi.h>

BatteryPopupService BatteryPopup;
const uint16_t BatteryPopupService::checkInterval = 5; //in seconds
ShutdownPopup *BatteryPopupService::shutdownPopup = nullptr;
WarningPopup *BatteryPopupService::warningPopup = nullptr;

void BatteryPopupService::loop(uint time){
	checkMicros += time;
	blinkMicros += time;
	if(Settings.get().shutdownTime != 0){
		autoShutdownMicros += time;
		if(autoShutdownMicros >= Settings.get().shutdownTime*1000000){
			if(tft != nullptr){
				tft->sleep();
			}
			ByteBoi.getExpander()->pinMode(BL_PIN, 1);
			WiFi.mode(WIFI_OFF);
			btStop();
			esp_deep_sleep_start();
			return;
		}
	}
	if(lastShutdownTime == (uint32_t)-1){
		autoShutdownMicros = 0;
		lastShutdownTime = Settings.get().shutdownTime;
	}
	if(lastShutdownTime != Settings.get().shutdownTime){
		autoShutdownMicros = 0;
	}


	//voltage not yet read
	if(Battery.getVoltage() == 0 || Battery.isCharging()){
		return;
	}
	uint8_t percentage = Battery.getPercentage();


	if(checkMicros >= checkInterval * 1000000){

		if(percentage <= 1){
			if(ContextTransition::isRunning() ||
			   (Modal::getCurrentModal() != nullptr && (Modal::getCurrentModal() == shutdownPopup || Modal::getCurrentModal() == warningPopup))
			   || ModalTransition::isRunning()) return;

			shutdownPopup = new ShutdownPopup(*Context::getCurrentContext());
			shutdownPopup->push(Context::getCurrentContext());
		}else if(percentage <= 15 && !warningShown){
			if(ContextTransition::isRunning() ||
			   (Modal::getCurrentModal() != nullptr && Modal::getCurrentModal() == shutdownPopup) ||
			   ModalTransition::isRunning()) return;

			warningShown = true;
			ModalTransition *transition;

			if(Modal::getCurrentModal() != nullptr){
				ModalTransition::setDeleteOnPop(false);
				transition = static_cast<ModalTransition *>((void *)Modal::getCurrentModal()->pop());
				transition->setDoneCallback([](Context *currentContext, Modal *prevModal){
					warningPopup = new WarningPopup(*currentContext);
					warningPopup->push(currentContext);
					warningPopup->returned(prevModal);
					ModalTransition::setDeleteOnPop(true);
				});
			}else{
				warningPopup = new WarningPopup(*Context::getCurrentContext());
				warningPopup->push(Context::getCurrentContext());
			}
		}
		checkMicros = 0;
	}
}

void BatteryPopupService::setTFT(TFT_eSPI* _tft){
	tft=_tft;
}

void BatteryPopupService::anyKeyPressed(){
	autoShutdownMicros = 0;
}
