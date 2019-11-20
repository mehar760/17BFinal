//
//  main.cpp
//
//  Created by william herrera.
//  Copyright © 2019 william herrera. All rights reserved.
//

#include <iostream>
#include <string>
#include "nano_engine.h"
using namespace std;

// Temporary Timer class taken from website

class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}

class GameState {
public:
	Nano_Texture* tex;
	Nano_Rect rect;

	Nano_Texture* tex2;
	Nano_Rect rect2;

	bool isquit = false;

	float player_x;
	float player_y;

	float player2_x = 50;
	float player2_y = 50;

	float v_x;
	float v_y;

	float v2_x;
	float v2_y;

	int scancode = 0;
	int grid[256 * 256];

	void draw();
	void getInput(Nano_Event& event, GameState& gamestate);	
	void move(float timeStep);	
	void update(float timeStep);
};

void GameState::draw() {
	tex = Nano_LoadTexture("Assets/blue_solid.bmp");
	rect = { 0, 0, 75, 75 };	    

	tex2 = Nano_LoadTexture("Assets/blue_solid.bmp");
	rect2 = { 0, 0, 75, 75 };
}

void GameState::getInput(Nano_Event& event, GameState& gamestate) {
	
	if (Nano_PollEvent(&event))
	{
		if (Nano_QuitEvent(&event)) {
			isquit = true;
		}
		if (Nano_KeyDownEvent(&event, &scancode))
		{
			switch (scancode) {
			case SDL_SCANCODE_RIGHT:
				if (v_y > 0 || v_y < 0) {
					v_y = 0;
				}
				gamestate.v_x += 200;
				break;
			case SDL_SCANCODE_LEFT:
				if (v_y > 0 || v_y < 0) {
					v_y = 0;
				}
				gamestate.v_x -= 200;
				break;
			case SDL_SCANCODE_UP:
				if (v_x > 0 || v_x < 0) {
					v_x = 0;
				}
				gamestate.v_y -= 200;
				break;
			case SDL_SCANCODE_DOWN:
				if (v_x > 0 || v_x < 0) {
					v_x = 0;
				}
				gamestate.v_y += 200;
				break;
			case SDL_SCANCODE_SPACE:
				isquit = true;
				break;
			}
		}
	}

	if (Nano_PollEvent(&event))
	{
		if (Nano_QuitEvent(&event)) {
			isquit = true;
		}
		if (Nano_KeyDownEvent(&event, &scancode))
		{
			switch (scancode) {
			case SDL_SCANCODE_D:
				if (v2_y > 0 || v2_y < 0) {
					v2_y = 0;
				}
				gamestate.v2_x += 200;
				break;
			
			case SDL_SCANCODE_A:
				if (v2_y > 0 || v2_y < 0) {
					v2_y = 0;
				}
				gamestate.v2_x -= 200;
				break;
			
			case SDL_SCANCODE_W:
				if (v2_x > 0 || v2_x < 0) {
					v2_x = 0;
				}
				gamestate.v2_y -= 200;
				break;
			case SDL_SCANCODE_S:
				if (v2_x > 0 || v2_x < 0) {
					v2_x = 0;
				}
				gamestate.v2_y += 200;
				break;
			case SDL_SCANCODE_SPACE:
				isquit = true;
				break;
			}
		}		
	}
}

void GameState::move(float timeStep) {
	player_x += v_x * timeStep;
	player_y += v_y * timeStep;

	player2_x += v2_x * timeStep;
	player2_y += v2_y * timeStep;
}

void GameState::update(float timeStep) {
	///render player		
	rect.x = player_x;
	rect.y = player_y;

	rect2.x = player2_x;
	rect2.y = player2_y;

	//mess with this to make trails?
	Nano_RenderClear();
	//=============================
	Nano_RenderCopy(tex, NULL, &rect);
	Nano_RenderCopy(tex2, NULL, &rect2);
	Nano_RenderPresent();
}

int main(int argc, char* argv[])
{
	GameState gamestate;
	GameState gamestate2;

	LTimer stepTimer;
	LTimer stepTimer2;
	
	Nano_Init();
	Nano_CreateRenderer("window", 100, 100, 640, 480);	
		
	//Event handler
	Nano_Event event;
	
	gamestate.draw();
	

	while (!gamestate.isquit)
	{
		//moving player
		gamestate.getInput(event, gamestate);			
		float timeStep = (stepTimer.getTicks() / 1000.f);				
		gamestate.move(timeStep);		

		//restart timer
		stepTimer.start();	

		gamestate.update(timeStep);
		
	}


	return 0;
}
