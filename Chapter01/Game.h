// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <ostream>
#include "SDL/SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 vPos;
	Vector2 vVel;
};

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window *mWindow;
	// Renderer for 2D drawing
	SDL_Renderer *mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	// Direction of paddle
	int mPaddleDir;
	int mPaddleDir2;
	// Position of paddle
	Vector2 mPaddlePos;
	Vector2 mPaddlePos2;
	std::vector<Ball> mBall;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Vector2& vect_name) {
	// Iterate over each element and print it
	for (const auto& element : vect_name) {
		os << element << " ";
	}
	return os;
}