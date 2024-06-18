// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <iostream>
#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDir(0), mPaddleDir2(0), mPaddlePos({ 0.0f, 0.0f }), mPaddlePos2({0.0f, 0.0f}) {}

std::ostream& operator<<(std::ostream& os, const Ball& b)
{
    return os << "ball vposx: " << b.vPos.x << "\nball vposx: " << b.vPos.y << "\nball vvelx: " << b.vVel.x << "\nball vvely: " << b.vVel.y << std::endl;
}

bool Game::Initialize()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)", // Window title
                               100,                                   // Top left x-coordinate of window
                               100,                                   // Top left y-coordinate of window
                               1024,                                  // Width of window
                               768,                                   // Height of window
                               0                                      // Flags (0 for no flags set)
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    //// Create SDL renderer
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mPaddlePos2.x = 1000.0f;
    mPaddlePos2.y = 768.0f / 2.0f;

    // multiball pong; get some
    Ball ball1;
    ball1.vPos = {1024.0f / 2.0f, 768.0f / 2.0f};
    ball1.vVel = {-200.0f, 235.0f};
    mBall.push_back(ball1);
    Ball ball2;
    ball2.vPos = {1024.0f / 3.0f, 768.0f / 3.0f};
    ball2.vVel = {-100.0f, 210.0f};
    mBall.push_back(ball2);
    for (const auto &ball : mBall)
    {
        std::cout << ball1 << std::endl;
        std::cout << ball2 << std::endl;
    }
    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // If we get an SDL_QUIT event, end loop
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    // Get state of keyboard
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    // Update paddle direction based on W/S keys
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }

    mPaddleDir2 = 0;
    if (state[SDL_SCANCODE_I])
    {
        mPaddleDir2 -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        mPaddleDir2 += 1;
    }
}

void Game::UpdateGame()
{
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();

    // Update paddle position based on direction
    if (mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddlePos.y < (paddleH / 2.0f + thickness))
        {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
        {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    if (mPaddleDir2 != 0)
    {
        mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddlePos2.y < (paddleH / 2.0f + thickness))
        {
            mPaddlePos2.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness))
        {
            mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    // Update ball position based on ball velocity
    int numBalls = mBall.size();
    for (int i = 0; i < numBalls; i++)
    {
        mBall[i].vPos.x += mBall[i].vVel.x * deltaTime;
        mBall[i].vPos.y += mBall[i].vVel.y * deltaTime;
    }

    // Bounce if needed
    // Did we intersect with the paddle?
    float b1DiffP1 = mPaddlePos.y - mBall[0].vPos.y;
    float b2DiffP1 = mPaddlePos.y - mBall[1].vPos.y;

    // Take absolute value of difference
    b1DiffP1 = (b1DiffP1 > 0.0f) ? b1DiffP1 : -b1DiffP1;
    b2DiffP1 = (b2DiffP1 > 0.0f) ? b2DiffP1 : -b2DiffP1;

    // Ball 1 Paddle 1
    if (
        // Our y-difference is small enough
        b1DiffP1 <= paddleH / 2.0f &&
        // We are in the correct x-position
        mBall[0].vPos.x <= 25.0f && mBall[0].vPos.x >= 20.0f &&
        // The ball is moving to the left
        mBall[0].vVel.x < 0.0f)
    {
        mBall[0].vVel.x *= -1.0f;
    }

    // Ball 2 Paddle 1
    if (
        // Our y-difference is small enough
        b2DiffP1 <= paddleH / 2.0f &&
        // We are in the correct x-position
        mBall[1].vPos.x <= 25.0f && mBall[1].vPos.x >= 20.0f &&
        // The ball is moving to the left
        mBall[1].vVel.x < 0.0f)
    {
        mBall[1].vVel.x *= -1.0f;
    }

    float b1DiffP2 = mPaddlePos2.y - mBall[0].vPos.y;
    float b2DiffP2 = mPaddlePos2.y - mBall[1].vPos.y;

    b1DiffP2 = (b1DiffP2 > 0.0f) ? b1DiffP2 : -b1DiffP2;
    b2DiffP2 = (b2DiffP2 > 0.0f) ? b2DiffP2 : -b2DiffP2;

    // Ball 1 Paddle 2
    if (
        // Our y-difference is small enough
        b1DiffP2 <= paddleH / 2.0f &&
        // We are in the correct x-position
        mBall[0].vPos.x >= 999.0f && mBall[0].vPos.x <= 1004.0f &&
        // The ball is moving to the right
        mBall[0].vVel.x > 0.0f)
    {
        mBall[0].vVel.x *= -1.0f;
    }

    // Ball 2 Paddle 2
    if (
        // Our y-difference is small enough
        b2DiffP2 <= paddleH / 2.0f &&
        // We are in the correct x-position
        mBall[1].vPos.x >= 999.0f && mBall[1].vPos.x <= 1004.0f &&
        // The ball is moving to the right
        mBall[1].vVel.x > 0.0f)
    {
        mBall[1].vVel.x *= -1.0f;
    }

    // Did the ball go off the screen? (if so, end game)
    else if (
        mBall[0].vPos.x <= 0.0f ||
        mBall[0].vPos.x >= 1005.0f ||
        mBall[1].vPos.x <= 0.0f ||
        mBall[1].vPos.x >= 1005.0f)
    {
        mIsRunning = false;
    }

    // Did ball1 collide with the top wall?
    else if (mBall[0].vPos.y <= thickness && mBall[0].vVel.y < 0.0f)
    {
        mBall[0].vVel.y *= -1;
    }

    // Did ball2 collide with the top wall?
    else if (mBall[1].vPos.y <= thickness && mBall[1].vVel.y < 0.0f)
    {
        mBall[1].vVel.y *= -1;
    }

    // Did ball1 collide with the bottom wall?
    else if (mBall[0].vPos.y >= (768 - thickness) && mBall[0].vVel.y > 0.0f)
    {
        mBall[0].vVel.y *= -1;
    }

    // Did ball2 collide with the bottom wall?
    else if (mBall[1].vPos.y >= (768 - thickness) && mBall[1].vVel.y > 0.0f)
    {
        mBall[1].vVel.y *= -1;
    }
}

void Game::GenerateOutput()
{
    // Set draw color to blue
    SDL_SetRenderDrawColor(mRenderer,
                           0,   // R
                           0,   // G
                           255, // B
                           255  // A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draw walls
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    // Draw top wall
    SDL_Rect wall{
        0,        // Top left x
        0,        // Top left y
        1024,     // Width
        thickness // Height
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // Draw bottom wall
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);

    // Draw right wall
    // wall.x = 1024 - thickness;
    // wall.y = 0;
    // wall.w = thickness;
    // wall.h = 1024;
    // SDL_RenderFillRect(mRenderer, &wall);

    // Draw paddle
    SDL_Rect paddle{static_cast<int>(mPaddlePos.x), static_cast<int>(mPaddlePos.y - paddleH / 2), thickness, static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &paddle);

    // Draw paddle for player 2
    SDL_Rect paddle2{static_cast<int>(mPaddlePos2.x), static_cast<int>(mPaddlePos2.y - paddleH / 2), thickness, static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &paddle2);

    // Draw ball1
    SDL_Rect ball{static_cast<int>(mBall[0].vPos.x - thickness / 2), static_cast<int>(mBall[0].vPos.y - thickness / 2), thickness, thickness};
    SDL_RenderFillRect(mRenderer, &ball);

    // Draw ball2
    SDL_Rect ball2{static_cast<int>(mBall[1].vPos.x - thickness / 2), static_cast<int>(mBall[1].vPos.y - thickness / 2), thickness, thickness};
    SDL_RenderFillRect(mRenderer, &ball2);

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
