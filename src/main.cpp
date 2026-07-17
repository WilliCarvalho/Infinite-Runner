/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <iostream>
#include <iterator>

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


bool isOnGround(AnimData data, int windowHeight)
{
	return data.position.y >= windowHeight - data.rectangle.height;
}

AnimData updatePlayerAnimData(AnimData playerAnim, float deltaTime)
{
	playerAnim.runningTime += deltaTime;
	if (playerAnim.runningTime >= playerAnim.updateTime && isOnGround(playerAnim, GetScreenHeight()))
	{
		playerAnim.runningTime = 0.f;
		playerAnim.rectangle.x = playerAnim.currentFrame * playerAnim.rectangle.width;
		//Adjusting frame position Y based on currentFrame (the first frame of walking animation is alone on it's row
		if (playerAnim.currentFrame >= 1)
		{
			playerAnim.rectangle.y = playerAnim.rectangle.width;
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

	return playerAnim;
}

AnimData updateObstacleAnimData(AnimData obstacle, float deltaTime)
{
	/*if (obstacle.position.x < -obstacle.rectangle.width)
	{
		obstacle.position.x = windowWidth;
	}*/

	//Update Obstacle animation frame
	obstacle.runningTime += deltaTime;
	if (obstacle.runningTime >= obstacle.updateTime)
	{
		obstacle.runningTime = 0.f;
		obstacle.rectangle.y = obstacle.currentFrame * obstacle.rectangle.height;

		obstacle.currentFrame++;

		if (obstacle.currentFrame >= 3)
		{
			obstacle.currentFrame = 1;
		}
	}
	return obstacle;
}

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
	int obstacleVelocity = -400;

	AnimData obstacles[6];

	for (int i = 0; i < std::size(obstacles); i++)
	{
		obstacles[i].rectangle.x = 0.f;
		obstacles[i].rectangle.y = obstacleTexture.height / 3;
		obstacles[i].rectangle.width = obstacleTexture.width;
		obstacles[i].rectangle.height = windowHeight - obstacleTexture.height / 3;
		obstacles[i].position.x = windowWidth + (300 * i);
		obstacles[i].position.y = windowHeight - obstacleTexture.height / 3;
		obstacles[i].currentFrame = 0;
		obstacles[i].updateTime = 1.f / 8.f;
		obstacles[i].runningTime = 0.f;
	}


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
	//TraceLog(LOG_INFO, "OBATACLE x POSITION 1: %f", obstacles[0].position.x);
	//TraceLog(LOG_INFO, "OBATACLE x POSITION 2: %f", obstacles[1].position.x);

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


		if (isOnGround(playerAnim, windowHeight))
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

		//Update  player animation frame
		

		//Update Obstacles
		for (int i = 0; i < std::size(obstacles); i++)
		{
			obstacles[i].position.x += obstacleVelocity * deltaTime;
			obstacles[i] = updateObstacleAnimData(obstacles[i], deltaTime);
			//DrawObstacle
			DrawTextureRec(obstacleTexture, obstacles[i].rectangle, obstacles[i].position, WHITE);
		}

		//DrawPlayer
		DrawTextureRec(PlayerTexture, playerAnim.rectangle, playerAnim.position, WHITE);


		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	UnloadTexture(obstacleTexture);
	CloseWindow();
	return 0;
}
