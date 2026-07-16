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

	//Resizing Obstacle Texture because it was too small
	Image img = LoadImage("2DTextures/Enemy/EnemySheet.png");
	int newWidth = static_cast<int>(img.width * 4);
	int newHeight = static_cast<int>(img.height * 4);
	ImageResizeNN(&img, newWidth, newHeight);

	//Obstacle Data
	Texture2D obstacleTexture = LoadTextureFromImage(img);
	Rectangle obstacleRec{ 0.f,obstacleTexture.height / 3, obstacleTexture.width, obstacleTexture.height / 3 };
	Vector2 obstaclePos{ windowWidth, windowHeight - obstacleRec.height };
	int obstacleVelocity = -400;
	int obstacleCurrentFrame = 1;
	const float obstacleUpdateFrameTime = 1.f / 6.f;
	float obstacleAnimRunningTime = 0.f;



	//Resizing Player Texture because it was too small
	Image img2 = LoadImage("2DTextures/Character/CharacterSheet.png");
	int newWidth2 = static_cast<int>(img2.width * 4);
	int newHeight2 = static_cast<int>(img2.height * 4);
	ImageResizeNN(&img2, newWidth2, newHeight2);

	//PLayer Data
	Texture2D PlayerTexture = LoadTextureFromImage(img2);
	UnloadImage(img2);

	Rectangle playerRectangle;
	playerRectangle.width = PlayerTexture.width / 4;
	playerRectangle.height = PlayerTexture.height / 6;
	//Rectangle position inside the spriteSheet (useful for animations)
	playerRectangle.x = 0;
	playerRectangle.y = 0;

	Vector2 playerPos;
	playerPos.x = windowWidth / 2 - playerRectangle.width / 2;
	playerPos.y = windowHeight - playerRectangle.height;
	int jumpForce = -600; //in pixels/s
	int yVelocity = 0;
	bool isInAir = false;

	//player animation frame data
	int playerCurrentFrame = 0;
	const float updateFrameTime = 1.f / 12.f; //Amount of time before update the animation (bigger numbers make animation faster)
	float playerAnimRunningTime = 0;


	//LOGS
	//TraceLog(LOG_INFO, "PLAYER RECTANGLE WIDTH: %f", playerRectangle.width);
	//TraceLog(LOG_INFO, "Player rectangle height: %f", playerRectangle.height);

	//Environment Data
	int floorPos = windowHeight - playerRectangle.height;
	const int gravityVelocity = 1'500; //acceleration due to gravity (pixels/s)/s
	float deltaTime = 0;


	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
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

		obstaclePos.x += obstacleVelocity * deltaTime;

		if (obstaclePos.x < -obstacleRec.width)
		{
			obstaclePos.x = windowWidth;
		}

		//Update  player animation frame
		playerAnimRunningTime += deltaTime;
		if (playerAnimRunningTime >= updateFrameTime && isInAir == false)
		{
			playerAnimRunningTime = 0.f;
			playerRectangle.x = playerCurrentFrame * playerRectangle.width;
			//Adjusting frame position Y based on currentFrame (the first frame of walking animation is alone on it's row
			if (playerCurrentFrame >= 1)
			{
				playerRectangle.y = PlayerTexture.height / 6;
			}
			else
			{
				playerRectangle.y = 0;
			}
			playerCurrentFrame++;
			if (playerCurrentFrame > 4)
			{
				playerCurrentFrame = 0;
			}
		}

		//Update Obstacle animation frame
		obstacleAnimRunningTime += deltaTime;
		if (obstacleAnimRunningTime >= obstacleUpdateFrameTime)
		{
			obstacleAnimRunningTime = 0.f;
			obstacleRec.y = obstacleCurrentFrame * obstacleRec.height;

			obstacleCurrentFrame++;

			if (obstacleCurrentFrame >= 3)
			{
				obstacleCurrentFrame = 1;
			}
		}

		//DrawPlayer
		DrawTextureRec(PlayerTexture, playerRectangle, playerPos, WHITE);

		//DrawObstacle
		DrawTextureRec(obstacleTexture, obstacleRec, obstaclePos, WHITE);

		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	UnloadTexture(obstacleTexture);
	CloseWindow();
	return 0;
}