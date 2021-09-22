#include "ShutdownPopup.h"
#include <Loop/LoopManager.h>
#include <ByteBoi.h>
#include <WiFi.h>
#include <SPIFFS.h>

const uint8_t ShutdownPopup::shutdownTime = 5;

ShutdownPopup::ShutdownPopup(Context &context) : Modal(context, 135, 60){
	batteryIconBuffer = static_cast<Color*>(ps_malloc(30 * 30 * 2));
	fs::File bgFile = SPIFFS.open("/launcher/off.raw");
	bgFile.read(reinterpret_cast<uint8_t*>(batteryIconBuffer), 30 * 30 * 2);
	bgFile.close();
	screen.getSprite()->setChroma(TFT_TRANSPARENT);
}
ShutdownPopup::~ShutdownPopup(){
	free(batteryIconBuffer);
}

void ShutdownPopup::draw(){
	Sprite& sprite = *screen.getSprite();

	sprite.clear(TFT_TRANSPARENT);
	sprite.fillRoundRect(0, 0, 135, 60, 10, TFT_BLACK);
	sprite.drawIcon(batteryIconBuffer, 5, 15, 30, 30, 1, TFT_TRANSPARENT);
	sprite.setTextColor(TFT_WHITE);
	sprite.setTextSize(1);
	sprite.setTextFont(2);
	sprite.setCursor(screen.getTotalX() + 40, screen.getTotalY() + 12);
	sprite.print("Battery empty!");
	sprite.setCursor(screen.getTotalX() + 40, screen.getTotalY() + 32);
	sprite.print("Shutting down");
}

void ShutdownPopup::start(){
	LoopManager::addListener(this);
	draw();
	screen.commit();
}

void ShutdownPopup::stop(){
	LoopManager::removeListener(this);
}

void ShutdownPopup::loop(uint micros){
	shutdownTimer += micros;
	if(shutdownTimer >= shutdownTime * 1000000){
		screen.getDisplay()->getTft()->sleep();
		ByteBoi.getExpander()->pinMode(BL_PIN, 1);
		WiFi.mode(WIFI_OFF);
		btStop();
		esp_deep_sleep_start();
	}
}

