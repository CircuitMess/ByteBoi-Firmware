#include "GameScroller.h"
#include <Display/Sprite.h>
#include <Loop/LoopManager.h>
#include "Elements/GameImage.h"
#include "GameManagement/GameManager.h"
#include <SPIFFS.h>
#include "GameInfo.hpp"
#include <SD.h>



GameScroller::GameScroller(Sprite* canvas, std::vector<LauncherItem>& items) : canvas(canvas), items(items),
		origin((canvas->width() - width) / 2){


	for(auto& item : items){
		item.image.setX(-width);
		item.image.setY(35);
	}

	// repos();
}

GameScroller::~GameScroller(){

}

void GameScroller::splash(float f){
	if(items.size() < 4) return;

	getCGame()->setX(f * (float) (origin + width) - width);
	getLGame()->setX(f * (float) (origin + width) - 2 * width - gutter);
	getRGame()->setX(f * (float) (origin + width + gutter - (origin + 2*width + 2*gutter)) + origin + 2*width + 2*gutter);
}

void GameScroller::reset(){
	selectedGame = 0;
	delta = 0;
	queued = false;

	LoopManager::removeListener(this);

	repos();
}

void GameScroller::repos(){
	for(auto& item : items){
		item.image.setX(-width);
		item.image.setY(35);
	}

	if(items.size() < 4) return;

	getLGame()->setX(origin - width - gutter);
	getCGame()->setX(origin);
	getRGame()->setX(origin + width + gutter);
}

uint8_t GameScroller::prev(){
	if(items.size() < 4) return 0;

	if(scrolling() && direction == PREV){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame < 2) ? items.size() - 2 + selectedGame : selectedGame - 2;
	}

	direction = PREV;

	if(scrolling()){ // direction == NEXT
		selectNext();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getLLGame()->setX(origin - 2 * width - 2 * gutter);
		LoopManager::addListener(this);
	}

	return (selectedGame < 1) ? items.size() - 1 : selectedGame - 1;
}

uint8_t GameScroller::next(){
	if(items.size() < 4) return 0;

	if(scrolling() && direction == NEXT){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame + 2) % items.size();
	}

	direction = NEXT;

	if(scrolling()){ // direction == PREV
		selectPrev();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getRRGame()->setX(origin + 2 * width + 2 * gutter);
		LoopManager::addListener(this);
	}

	return (selectedGame + 1) % items.size();
}

void GameScroller::draw(){
	if(items.size() < 4) return;

	getCGame()->draw();
	getRGame()->draw();
	getLGame()->draw();

	if(scrolling()){
		if(direction == NEXT){
			getRRGame()->draw();
		}else{
			getLLGame()->draw();
		}
	}
}

void GameScroller::loop(uint micros){
	if(items.size() < 4){
		reset();
		LoopManager::removeListener(this);
		return;
	}

	delta += speed * (micros / 1000000.0f) * (float) multiplier;

	if(direction == PREV){
		getLLGame()->setX(origin - 2 * width - 2 * gutter + delta);
		getLGame()->setX(origin - width - gutter + delta);
		getCGame()->setX(origin + delta);
		getRGame()->setX(origin + width + gutter + delta);
	}else{
		getRRGame()->setX(origin + 2 * width + 2 * gutter - delta);
		getRGame()->setX(origin + width + gutter - delta);
		getCGame()->setX(origin - delta);
		getLGame()->setX(origin - width - gutter - delta);
	}

	if(delta >= (width + gutter)){
		if(direction == NEXT){
			selectNext();
		}else{
			selectPrev();
		}

		if(queued){
			queued = false;
			delta = 1;
			return;
		}

		LoopManager::removeListener(this);
		delta = 0;
		repos();
	}
}

bool GameScroller::scrolling(){
	return delta != 0;
}

void GameScroller::selectNext(){
	selectedGame = (selectedGame + 1) % items.size();
}

void GameScroller::selectPrev(){
	if(selectedGame == 0){
		selectedGame = items.size() - 1;
	}else{
		selectedGame--;
	}
}

GameImage* GameScroller::getCGame(){
	return &items[selectedGame].image;
}

GameImage* GameScroller::getLGame(){
	if(selectedGame == 0) return &items.back().image;
	return &items[selectedGame - 1].image;
}

GameImage* GameScroller::getRGame(){
	return &items[(selectedGame + 1) % items.size()].image;
}

GameImage* GameScroller::getLLGame(){
	if(selectedGame <= 1) return &items[items.size() - 2 + selectedGame].image;
	return &items[selectedGame - 2].image;
}

GameImage* GameScroller::getRRGame(){
	return &items[(selectedGame + 2) % items.size()].image;
}

GameInfo* const GameScroller::getSelectedGame() const{
	return Games.getGames()[selectedGame];

}
