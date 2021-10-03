#include <Loop/LoopManager.h>
#include "LoadingIndicator.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "GameScroller.h"
#include <ByteBoi.h>
#include "ErrorModal.h"
#include "GameManagement/GameManager.h"

LoadingIndicator::LoadingIndicator(Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title) : canvas(canvas), logo(logo), scroller(scroller), title(title){}

void LoadingIndicator::start(GameInfo* game, GameImage* image){
	if(state != OUT) return;
	this->game = game;
	this->image = image;
	imageCopy = *image;
	currentText = title->getCurrent();
	title->change("Loading...");
	logo->stop();
	state = ENTER;
	f = 0;
	LoopManager::addListener(this);
}

void LoadingIndicator::stop(){
	if(state == OUT || state == EXIT) return;
	if(state == IN){
		*image = imageCopy;
		imageCopy = GameImage();
		image = nullptr;
	}
	game = nullptr;
	title->change(currentText);
	logo->start();
	state = EXIT;
	loadJob = nullptr;
	LoopManager::addListener(this);
}

void LoadingIndicator::finish(){
	state = FINISH;
	finishTime = millis();
	*image = imageCopy;
	title->change(currentText);

	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Games.setGameListener(nullptr);
}

void LoadingIndicator::abort(){
	if(state != IN && state != ENTER) return;
	if(loadJob != nullptr){
		loadJob->abort();
	}
	stop();
}

void LoadingIndicator::loop(uint micros){
	// abandon hope all ye who enter here

	if(loadJob && loadJob->hasError() && state == IN){
		auto* job = loadJob;
		stop();
		loadJob = job;
	}

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
				state = OUT;
				logo->setCentered(0);
				scroller->load(0);

				if(image && imageCopy){
					*image = imageCopy;
					imageCopy = GameImage();
					image = nullptr;
				}

				if(loadJob && loadJob->hasError()){
					Modal* errorModal = new ErrorModal(*Context::getCurrentContext(), loadJob->getError());
					errorModal->push(Context::getCurrentContext());
					loadJob = nullptr;
				}

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
			loadJob = Loader.loadGame(game);
			return;
		}
	}else if(state == IN){
		if(loadJob->isDone() && !loadJob->hasError()){
			finish();
		}

		uint32_t m = millis();

		if(m - lastDraw < 200){
			for(int y = 2 + 60.0f * (1.0f - loadJob->getProgress()); y < 62; y++){
				for(int x = 2; x < 62; x++){
					size_t i = y * 64 + x;
					image->getBuffer()[i] = ((bool) image->getBuffer()[i]) * imageCopy.getBuffer()[i];
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
				size_t i = y * 64 + x;
				bool c = field[(int) floor((x-2) / 6)][(int) floor((y-2) / 6)];
				bool under = (1.0f - (float) y / 60.0f) < loadJob->getProgress();
				if(c){
					image->getBuffer()[i] = TFT_BLACK;
				}else if(under){
					image->getBuffer()[i] = imageCopy.getBuffer()[i];
				}else{
					lgfx::rgb565_t c(imageCopy.getBuffer()[i]);
					c.R8(min(255.0f, c.R8() + (float) (255 - c.R8()) * 0.6f));
					c.G8(min(255.0f, c.G8() + (float) (255 - c.G8()) * 0.6f));
					c.B8(min(255.0f, c.B8() + (float) (255 - c.B8()) * 0.6f));
					image->getBuffer()[i] = c.operator unsigned short();
				}
			}
		}
	}

	if(state == ENTER || state == EXIT){
		logo->setCentered(-f);
		scroller->load(f);

		if(image && imageCopy){
			for(int x = 2; x < 62; x++){
				for(int y = 2; y < 62; y++){
					size_t i = y * 64 + x;

					lgfx::rgb565_t c(imageCopy.getBuffer()[i]);
					c.R8(min(255.0f, c.R8() + (float) (255 - c.R8()) * f * 0.7f));
					c.G8(min(255.0f, c.G8() + (float) (255 - c.G8()) * f * 0.7f));
					c.B8(min(255.0f, c.B8() + (float) (255 - c.B8()) * f * 0.7f));

					image->getBuffer()[i] = c.operator unsigned short();
				}
			}
		}
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
	return state != OUT;
}

bool LoadingIndicator::isBooting() const{
	return state == FINISH;
}

void LoadingIndicator::setCanvas(Sprite* canvas){
	LoadingIndicator::canvas = canvas;
}
