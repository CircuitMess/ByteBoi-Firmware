#include "IntroScreen.h"
#include "Launcher.h"
#include <FS.h>
#include <Loop/LoopManager.h>
#include <FS/CompressedFile.h>
#include <SPIFFS.h>

IntroScreen* IntroScreen::instance = nullptr;

IntroScreen::IntroScreen(Display& display) : Context(display){
	instance = this;

	fs::File f = SPIFFS.open("/launcher/intro.g565.hs");
	if(!f){
		Serial.println("Error opening intro gif");
		return;
	}

	gif = new AnimatedSprite(screen.getSprite(), CompressedFile::open(f, 14, 12,1024));
	gif->setSwapBytes(true);
	gif->setXY(0, 0);

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

		Display& display = *instance->getScreen().getDisplay();

		instance->stop();
		delete instance;
		Launcher* launcher = new Launcher(&display);
		launcher->unpack();
		launcher->start();
	});

	LoopManager::addListener(this);

	draw();
	screen.commit();
}

void IntroScreen::stop(){
	LoopManager::removeListener(this);
}

void IntroScreen::loop(uint micros){
	if(gif && gif->checkFrame()){
		draw();
		screen.commit();
	}
}