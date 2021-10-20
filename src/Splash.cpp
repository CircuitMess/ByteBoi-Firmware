#include "Splash.h"
#include "Elements/Logo.h"
#include "GameScroller.h"
#include <Audio/Piezo.h>

Splash::Splash(GameScroller* scroller, Logo* logo) : scroller(scroller), logo(logo){

}

void Splash::loop(uint micros){
	if(f == 0){
		f += 0.001;
		return;
	}

	f = min(1.0, f + micros / 500000.0);

	scroller->splash(f);
	logo->setCentered(1.0f - f);
}

bool Splash::done(){
	return f >= 1;
}
