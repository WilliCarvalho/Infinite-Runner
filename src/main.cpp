/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main()
{
	const int windowWidth = 800;
	const int windowHeight = 600;

	InitWindow(windowWidth, windowHeight, "Infinite Runner do Wilzin");

	SearchAndSetResourceDir("resources");


	//Resizing Texture because it was too small
	Image img = LoadImage("2DTextures/Character/CharacterSheet.png");
	int newWidth = static_cast<int>(img.width * 4);
	int newHeight = static_cast<int>(img.height * 4);
	ImageResizeNN(&img, newWidth, newHeight);

	//PLayer Data
	Texture2D PlayerTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	Rectangle playerRectangle;
	playerRectangle.width = PlayerTexture.width / 4;
	playerRectangle.height = PlayerTexture.height / 6;
	//Rectangle position inside the spriteSheet (useful for animations)
	playerRectangle.x = 0;
	playerRectangle.y = 0;

	Vector2 playerPos;
	playerPos.x = windowWidth / 2 - playerRectangle.width / 2;
	playerPos.y = windowHeight - playerRectangle.height;

	//player animation frame data
	int currentFrame = 0;
	const float UpdateFrameTime = 1.f / 12.f; //Amount of time before update the animation

	int jumpForce = -600; //in pixels/s
	int yVelocity = 0;
	bool isInAir = false;

	//LOGS
	//TraceLog(LOG_INFO, "PLAYER RECTANGLE WIDTH: %f", playerRectangle.width);
	//TraceLog(LOG_INFO, "Player rectangle height: %f", playerRectangle.height);

	//Environment Data
	int floorPos = windowHeight - playerRectangle.height;
	const int gravityVelocity = 1'500; //acceleration due to gravity (pixels/s)/s
	float deltaTime = 0;
	float runningTime = 0;


	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
		runningTime += deltaTime;
		BeginDrawing();

		ClearBackground(GREEN);


		if (playerPos.y >= floorPos)
		{
			//is in the ground
			yVelocity = 0;
			isInAir = false;
		}
		else
		{
			//Is in the air
			yVelocity += gravityVelocity * deltaTime;
			isInAir = true;
		}

		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			yVelocity += jumpForce;
		}

		playerPos.y += yVelocity * deltaTime;

		//Update animation frame
		if (runningTime >= UpdateFrameTime)
		{
			runningTime = 0.f;
			playerRectangle.x = currentFrame * playerRectangle.width;
			//Adjusting frame position Y based on currentFrame (the first frame of walking animation is alone on it's row
			if (currentFrame >= 1)
			{
				playerRectangle.y = PlayerTexture.height / 6;
			}
			else
			{
				playerRectangle.y = 0;
			}
			currentFrame++;
			if (currentFrame > 4)
			{
				currentFrame = 0;
			}
		}

		DrawTextureRec(PlayerTexture, playerRectangle, playerPos, WHITE);

		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	CloseWindow();
	return 0;
}