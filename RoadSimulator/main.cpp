/***********************
  Road Simulator

  SDL 1.2 + OpenGL
  stb_truetype.h public domain C truetype rasterization library
      http://nothings.org/stb/stb_truetype.h

************************
*/

// View
#include "roadview.h"
#include "carview.h"
#include "rulesetview.h"

// Model
#include "car.h"
#include "road.h"
#include "ruleset.h"

// Controller
#include "sdlcontroller.h"

// Helper classes for Observer & etc
#include "utils.hpp"
#include "everyframenotifier.hpp"
#include "config.h"

//#include <SDL/SDL.h>
//#undef main

int main(int argc, char *args[])
{
	try
	{
		Config::get().loadConf("config.txt");

		SDLController render;
		render.init();

		// create models
		Road rmodel;
		rmodel.setWidthHeight(
					(float)Config::get().data.SCREEN_WIDTH/2-20,
					(float)Config::get().data.SCREEN_HEIGHT-20);
		Car cmodel;
		RuleSet rules;
		rmodel.attach(rules);
		cmodel.attach(rules);

		cmodel.setPos( Config::get().data.CAR_START_X, Config::get().data.CAR_START_Y,
					   Config::get().data.CAR_START_ANGLE );
		rmodel.setLength(6);

		for (int i=0; i<6; i++)
			rmodel.setRoad( i, (i==1 || i==4)?1:0 );

		// create views
		RoadView rview(10, 10,
					   (float)Config::get().data.SCREEN_WIDTH/2.0f, (float)Config::get().data.SCREEN_HEIGHT - 10);

		CarView cview(10, &rview);
		RuleSetView rulesView(Config::get().data.SCREEN_WIDTH/2.0f + 10, 10);

		// connect models and views
		rmodel.attach(rview);
		cmodel.attach(cview);
		rules.attach(rulesView);

		// connect view and controller
		render.addView(&rview);
		render.addView(&cview);
		render.addView(&rulesView);

		// connect model and controller
		render.getFrameNotifier().attach(cmodel);
		render.getKeyListener().addListener(dynamic_cast<Observer<KeyEvent> &>(cmodel));

		render.gameCycle();
	} catch(RuleSetView::FontFileNotFound) {
		MessageBoxA(NULL, "Font file not found at RuleSetView::initFont", APP_NAME, NULL);
	} catch(RuleSet::BadRuleProgram) {
		MessageBoxA(NULL, "Eternal cycle catch at RuleSet program", APP_NAME, NULL);
	}

	return 0;
}

