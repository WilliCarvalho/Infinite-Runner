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


bool IsOnGround(AnimData data, int windowHeight)
{
	return data.position.y >= windowHeight - data.rectangle.height;
}

AnimData UpdatePlayerAnimData(AnimData playerAnim, float deltaTime, Texture2D playerTexture)
{
	playerAnim.runningTime += deltaTime;
	if (playerAnim.runningTime >= playerAnim.updateTime && IsOnGround(playerAnim, GetScreenHeight()))
	{
		playerAnim.runningTime = 0.f;
		playerAnim.rectangle.x = playerAnim.currentFrame * playerAnim.rectangle.width;
		//Adjusting frame position Y based on currentFrame (the first frame of walking animation is alone on it's row
		if (playerAnim.currentFrame >= 1)
		{
			playerAnim.rectangle.y = playerTexture.height / 6;
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

AnimData UpdateObstacleAnimData(AnimData obstacle, float deltaTime)
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

void DrawBackground(Texture2D texture, float drawPosition)
{
	Vector2 background1Pos(drawPosition, 0.f);
	DrawTextureEx(texture, background1Pos, 0.f, 2.f, WHITE);
	Vector2 background2Pos(drawPosition + texture.width * 2, 0.f);
	DrawTextureEx(texture, background2Pos, 0.f, 2.f, WHITE);
}

Texture2D ResizeTexture(const char *fileName, int newSize)
{
	Image img = LoadImage(fileName);
	int newWidth = static_cast<int>(img.width * newSize);
	int newHeight = static_cast<int>(img.height * newSize);
	ImageResizeNN(&img, newWidth, newHeight);
	Texture2D resizedTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	return resizedTexture;
}

int main()
{
	const int windowWidth = 640;
	const int windowHeight = 360;

	InitWindow(windowWidth, windowHeight, "Infinite Runner do Wilzin");

	SearchAndSetResourceDir("resources");


	//Obstacle Data
	Texture2D obstacleTexture = ResizeTexture("2DTextures/Enemy/EnemySheet.png", 4); //Resizing Obstacle Texture because it was too small
	int obstacleVelocity = -400;

	AnimData obstacles[6];

	for (int i = 0; i < std::size(obstacles); i++)
	{
		obstacles[i].rectangle.x = 0.f;
		obstacles[i].rectangle.y = obstacleTexture.height / 3;
		obstacles[i].rectangle.width = obstacleTexture.width;
		obstacles[i].rectangle.height = obstacleTexture.height / 3;
		obstacles[i].position.x = windowWidth + (300 * i);
		obstacles[i].position.y = windowHeight - obstacleTexture.height / 3;
		obstacles[i].currentFrame = 0;
		obstacles[i].updateTime = 1.f / 8.f;
		obstacles[i].runningTime = 0.f;
	}

	//PLayer Data
	Texture2D PlayerTexture = ResizeTexture("2DTextures/Character/CharacterSheet.png", 4);
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
	const int gravityVelocity = 1'500; //acceleration due to gravity (pixels/s)/s
	float deltaTime;
	Texture2D backgroundTexture = LoadTexture("2DTextures/background/Background5.png");
	Texture2D midgroundTexture = LoadTexture("2DTextures/background/Background3.png");
	Texture2D foregroundTexture = LoadTexture("2DTextures/background/Background2.png");
	Texture2D cloudTexture = LoadTexture("2DTextures/background/Background4.png");
	float backgroundPosX = 0.f;
	float midgroundPosX = 0.f;
	float foregroundPosX = 0.f;
	float cloudPosX = 0.f;

	Texture2D finishLineTexture = ResizeTexture("2DTextures/Sprites/Textures/Decor/Decor.png", 3);;
	Rectangle spritePosition{ 0.f, 45.f * 3, 26.f * 3, 50.f * 3 };
	Vector2 finishLinePosition{ obstacles[std::size(obstacles) - 1].position.x + 500, windowHeight - spritePosition.height};
	


	//GAME LOOP
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
		BeginDrawing();

		ClearBackground(GREEN);

		backgroundPosX -= 20.f * deltaTime;
		midgroundPosX -= 40.f * deltaTime;
		foregroundPosX -= 80.f * deltaTime;
		cloudPosX -= 35.f * deltaTime;
		if (backgroundPosX <= -backgroundTexture.width * 2)
		{
			backgroundPosX = 0.f;
		}

		if (midgroundPosX <= -midgroundTexture.width * 2)
		{
			midgroundPosX = 0.f;
		}

		if (foregroundPosX <= -foregroundTexture.width *2)
		{
			foregroundPosX = 0.f;
		}

		if (cloudPosX <= -cloudTexture.width * 2)
		{
			cloudPosX = 0.f;
		}

		//draw backgrounds
		DrawBackground(backgroundTexture, backgroundPosX);
		DrawBackground(midgroundTexture, midgroundPosX);
		DrawBackground(foregroundTexture, foregroundPosX);
		DrawBackground(cloudTexture, cloudPosX);


		if (IsOnGround(playerAnim, windowHeight))
		{
			//is in the ground
			yVelocity = 0;
			playerAnim.position.y = windowHeight - (PlayerTexture.height / 6);
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
		playerAnim = UpdatePlayerAnimData(playerAnim, deltaTime, PlayerTexture);

		//Update Obstacles
		for (int i = 0; i < std::size(obstacles); i++)
		{
			obstacles[i].position.x += obstacleVelocity * deltaTime;
			obstacles[i] = UpdateObstacleAnimData(obstacles[i], deltaTime);
			//DrawObstacle
			DrawTextureRec(obstacleTexture, obstacles[i].rectangle, obstacles[i].position, WHITE);
		}

		finishLinePosition.x += obstacleVelocity * deltaTime;

		//Draw FinishLine
		DrawTextureRec(finishLineTexture, spritePosition, finishLinePosition, WHITE);

		//DrawPlayer
		DrawTextureRec(PlayerTexture, playerAnim.rectangle, playerAnim.position, WHITE);

		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	UnloadTexture(obstacleTexture);
	UnloadTexture(backgroundTexture);
	UnloadTexture(midgroundTexture);
	UnloadTexture(foregroundTexture);
	UnloadTexture(cloudTexture);
	CloseWindow();
	return 0;
}
