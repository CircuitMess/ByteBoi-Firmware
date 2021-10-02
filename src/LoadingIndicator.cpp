#include <Loop/LoopManager.h>
#include "LoadingIndicator.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "GameScroller.h"
#include "GameManagement/GameLoader.h"
#include <ByteBoi.h>

LoadingIndicator::LoadingIndicator(Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title) : canvas(canvas), logo(logo), scroller(scroller), title(title){}

void LoadingIndicator::start(GameInfo* game, GameImage* image){
	if(state != OUT) return;
	this->game = game;
	this->image = image;
	imageCopy = *image;
	currentText = title->getCurrent();
	title->change("Loading...");
	active = true;
	state = ENTER;
	f = 0;
	LoopManager::addListener(this);
}

void LoadingIndicator::stop(){
	if(state == OUT || state == EXIT) return;
	*image = imageCopy;
	imageCopy = GameImage();
	image = nullptr;
	game = nullptr;
	title->change(currentText);
	state = EXIT;
	LoopManager::addListener(this);
}

void LoadingIndicator::finish(){
	state = FINISH;
	finishTime = millis();
	*image = imageCopy;
	title->change(currentText);
}

void LoadingIndicator::loop(uint micros){
	// abandon hope all ye who enter here

	if(boot){
		ByteBoi.fadeout();
		ByteBoi.getExpander()->pinWrite(BL_PIN, HIGH);
		Loader.boot();
	}

	if(state == OUT){
		LoopManager::removeListener(this);
		return;
	}

	if(state == EXIT || state == FINISH){
		if(state == FINISH && millis() - finishTime < 2000) return;
		if(state == FINISH && currentText != ""){
			title->change("");
			currentText = "";
		}

		f -= (float) micros / 500000.0f;

		if(f < 0){
			if(state == EXIT){
				f = 0;
				active = false;
				state = OUT;
				logo->setCentered(0);
				scroller->load(0);
				LoopManager::removeListener(this);
				return;
			}else{
				f = 0;
				boot = true;
			}
		}
	}else if(state == ENTER){
		f += (float) micros / 500000.0f;

		if(f >= 1){
			f = 1;
			state = IN;
			logo->setCentered(-1);
			scroller->load(1);
			lastDraw = 0;
			Loader.loadGame(game);
			return;
		}
	}else if(state == IN){
		uint32_t m = millis();

		if(m - lastDraw < 200){
			for(int y = 2 + 60.0f * (1.0f - Loader.getProgress()); y < 62; y++){
				for(int x = 2; x < 62; x++){
					image->getBuffer()[y * 64 + x] = ((bool) image->getBuffer()[y * 64 + x]) * 0x0041ff;
				}
			}

			return;
		}

		lastDraw = m;

		bool field[10][10];
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 10; j++){
				field[i][j] = rand() % 2;
			}
		}

		for(int x = 2; x < 62; x++){
			for(int y = 2; y < 62; y++){
				bool c = field[(int) floor((x-2) / 6)][(int) floor((y-2) / 6)];
				bool under = (1.0f - (float) y / 60.0f) < Loader.getProgress();
				if(c){
					image->getBuffer()[y * 64 + x] = TFT_BLACK;
				}else{
					image->getBuffer()[y * 64 + x] = under ? 0x0041ff : TFT_WHITE;
				}
			}
		}
	}

	if(state == ENTER || state == EXIT){
		logo->setCentered(-f);
		scroller->load(f);
	}else if(state == FINISH){
		logo->setCentered(-f * 2.0f + 1.0f);
		scroller->finish(1.0f - f);
	}
}

void LoadingIndicator::draw(){
	if(state != EXIT && state != IN && state != FINISH) return;
	if(boot) return;

}

bool LoadingIndicator::isActive() const{
	return active;
}

void LoadingIndicator::setCanvas(Sprite* canvas){
	LoadingIndicator::canvas = canvas;
}
