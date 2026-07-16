/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

struct AnimData
{
	Rectangle rectangle;
	Vector2 position;
	int currentFrame;
	float updateTime;
	float runningTime;
};

struct GameOBjectData
{
	Texture2D texture;
	AnimData animData;
	float moveSpeed;
};

int main()
{
	const int windowWidth = 512;
	const int windowHeight = 380;

	InitWindow(windowWidth, windowHeight, "Infinite Runner do Wilzin");

	SearchAndSetResourceDir("resources");

	//Resizing Obstacle Texture because it was too small
	Image img = LoadImage("2DTextures/Enemy/EnemySheet.png");
	int newWidth = static_cast<int>(img.width * 4);
	int newHeight = static_cast<int>(img.height * 4);
	ImageResizeNN(&img, newWidth, newHeight);

	//Obstacle Data
	Texture2D obstacleTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	AnimData obstacleAnim{
		{ 0.f,obstacleTexture.height / 3, obstacleTexture.width, obstacleTexture.height / 3 }, //Rectangle
		{windowWidth, windowHeight - obstacleTexture.height/3}, //position
		0, //currentFrame
		1.f / 8.f, //updateTime
		0 //runningTime
	};
	int obstacleVelocity = -400;



	//Resizing Player Texture because it was too small
	Image img2 = LoadImage("2DTextures/Character/CharacterSheet.png");
	int newWidth2 = static_cast<int>(img2.width * 4);
	int newHeight2 = static_cast<int>(img2.height * 4);
	ImageResizeNN(&img2, newWidth2, newHeight2);

	//PLayer Data
	Texture2D PlayerTexture = LoadTextureFromImage(img2);
	UnloadImage(img2);

	AnimData playerAnim{
		{0.f, 0.f, PlayerTexture.width / 4, PlayerTexture.height / 6},
		{windowWidth / 2 - PlayerTexture.width / 4, windowHeight - (PlayerTexture.height / 6)},
		0,
		1.f / 12.f,
		0.f
	};

	int jumpForce = -600; //in pixels/s
	int yVelocity = 0;
	bool isInAir = false;


	//LOGS
	//TraceLog(LOG_INFO, "PLAYER RECTANGLE WIDTH: %f", playerRectangle.width);
	//TraceLog(LOG_INFO, "Player rectangle height: %f", playerRectangle.height);

	//Environment Data
	int floorPos = windowHeight - playerAnim.rectangle.height;
	const int gravityVelocity = 1'500; //acceleration due to gravity (pixels/s)/s
	float deltaTime = 0;


	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
		BeginDrawing();

		ClearBackground(GREEN);


		if (playerAnim.position.y >= floorPos)
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

		playerAnim.position.y += yVelocity * deltaTime;

		obstacleAnim.position.x += obstacleVelocity * deltaTime;

		if (obstacleAnim.position.x < -obstacleAnim.rectangle.width)
		{
			obstacleAnim.position.x = windowWidth;
		}

		//Update  player animation frame
		playerAnim.runningTime += deltaTime;
		if (playerAnim.runningTime >= playerAnim.updateTime && isInAir == false)
		{
			playerAnim.runningTime = 0.f;
			playerAnim.rectangle.x = playerAnim.currentFrame * playerAnim.rectangle.width;
			//Adjusting frame position Y based on currentFrame (the first frame of walking animation is alone on it's row
			if (playerAnim.currentFrame >= 1)
			{
				playerAnim.rectangle.y = PlayerTexture.height / 6;
			}
			else
			{
				playerAnim.rectangle.y = 0;
			}
			
			playerAnim.currentFrame++;

			if (playerAnim.currentFrame > 4)
			{
				playerAnim.currentFrame = 0;
			}
		}

		//Update Obstacle animation frame
		obstacleAnim.runningTime += deltaTime;
		if (obstacleAnim.runningTime >= obstacleAnim.updateTime)
		{
			obstacleAnim.runningTime = 0.f;
			obstacleAnim.rectangle.y = obstacleAnim.currentFrame * obstacleAnim.rectangle.height;

			obstacleAnim.currentFrame++;

			if (obstacleAnim.currentFrame >= 3)
			{
				obstacleAnim.currentFrame = 1;
			}
		}

		//DrawPlayer
		DrawTextureRec(PlayerTexture, playerAnim.rectangle, playerAnim.position, WHITE);

		//DrawObstacle
		DrawTextureRec(obstacleTexture, obstacleAnim.rectangle, obstacleAnim.position, WHITE);

		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	UnloadTexture(obstacleTexture);
	CloseWindow();
	return 0;
}