#include <Loop/LoopManager.h>
#include "LoadingIndicator.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "GameScroller.h"
#include "GameManagement/GameLoader.h"

LoadingIndicator::LoadingIndicator(Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title) : canvas(canvas), logo(logo), scroller(scroller), title(title){}

void LoadingIndicator::start(){
	if(state != OUT) return;
	currentText = title->getCurrent();
	title->change("Loading...");
	active = true;
	state = ENTER;
	f = 0;
	LoopManager::addListener(this);
}

void LoadingIndicator::stop(){
	if(state == OUT || state == EXIT) return;
	if(state == IN){
		f = 1;
		ballf = 0.999f;
	}
	title->change(currentText);
	state = EXIT;
	LoopManager::addListener(this);
}

void LoadingIndicator::finish(){
	state = FINISH;
}

void LoadingIndicator::loop(uint micros){
	if(boot){
		Loader.boot();
	}

	if(state == OUT){
		LoopManager::removeListener(this);
		return;
	}

	if(state == EXIT){
		f -= (float) micros / 500000.0f;

		if(f < 0){
			active = false;
			state = OUT;
			logo->setCentered(0);
			scroller->splash(1);
			LoopManager::removeListener(this);
			return;
		}
	}else{
		float multiplier = 1;
		if(state == IN || state == FINISH){
			multiplier = 0.25;
		}

		f += multiplier * (float) micros / 500000.0f;

		if(f >= 1){
			f -= 1.0f;

			if(state == ENTER){
				logo->setCentered(1);
				scroller->splash(0);
				state = IN;
				ballf = 0.001f;
			}
		}

		if(state == FINISH){
			ballf -= (float) micros / 1000000.0f;
			if(ballf <= 0){
				// we need another loop iteration to draw splash screen without ball
				ballf = 0;
				boot = true;
				return;
			}
		}else if(ballf != 0 && ballf < 1){
			ballf += (float) micros / 500000.0f;
			if(ballf >= 1){
				ballf = 1;
			}
		}
	}

	if(state == ENTER || state == EXIT){
		logo->setCentered(f);
		scroller->splash(1.0f - f);
		return;
	}
}

void LoadingIndicator::draw(){
	if(state != IN && state != FINISH) return;
	if(boot) return;

	int16_t ballX = sin(f * M_PI * 2.0f) * 60.0f * ballf;
	int16_t ballY = cos(f * M_PI * 2.0f) * 30.0f * ballf;
	
	canvas->fillCircle(canvas->width() / 2 + ballX, canvas->height() / 2 - ballY, r, C_HEX(0x0082ff));
}

bool LoadingIndicator::isActive() const{
	return active;
}
