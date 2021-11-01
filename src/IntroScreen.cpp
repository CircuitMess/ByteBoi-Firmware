#include "IntroScreen.h"
#include "Launcher.h"
#include <FS.h>
#include <Loop/LoopManager.h>
#include <FS/CompressedFile.h>
#include <FS/RamFile.h>
#include <SPIFFS.h>
#include <Playback/PlaybackSystem.h>

IntroScreen* IntroScreen::instance = nullptr;

IntroScreen::IntroScreen(Display& display, Context* menu) : Context(display), menu(menu){
	instance = this;

	fs::File f = SPIFFS.open("/launcher/intro/intro.g565.hs");
	if(!f){
		Serial.println("Error opening intro gif");
		return;
	}

	gif = new AnimatedSprite(screen.getSprite(), CompressedFile::open(RamFile::open(f), 14, 12,1024));
	gif->setSwapBytes(true);
	gif->setXY(0, 0);

	intro = new Sample(SPIFFS.open("/launcher/intro/intro.aac"), true);
	intro->setLooping(false);

	IntroScreen::pack();
}

IntroScreen::~IntroScreen(){
	delete gif;
	instance = nullptr;
}

void IntroScreen::draw(){
	if(gif == nullptr){
		Serial.println("Intro gif error");
		return;
	}
	gif->nextFrame();
	gif->push();
}

void IntroScreen::start(){
	if(!gif) return;

	gif->setLoopDoneCallback([]{
		if(instance == nullptr) return;

		Context* menu = instance->menu;

		instance->stop();
		delete instance;

		if(menu){
			menu->unpack();
			menu->start();
		}
	});

	LoopManager::addListener(this);
	Playback.play(intro);

	draw();
	screen.commit();
}

void IntroScreen::stop(){
	LoopManager::removeListener(this);
	LED.setRGB(LEDColor::OFF);
}

void IntroScreen::loop(uint micros){
	if(millis() - previousTime >= (colorCounter > 7 ? colorHoldTime : 200)){
		previousTime = millis();

		LEDColor color;

		if(colorCounter <= 7){
			do {
				color = static_cast<LEDColor>(random(1, 7));
			} while(color == lastColor);
			colorCounter++;
		}else{
			if(lastColor == LEDColor::WHITE){
				color = LEDColor::OFF;
			}else{
				color = LEDColor::WHITE;
			}
			colorHoldTime += 10;
		}

		LED.setRGB(color);
		lastColor = color;
	}
	if(gif && gif->checkFrame()){
		draw();
		screen.commit();
	}
}