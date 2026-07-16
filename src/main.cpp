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
	int newWidth = static_cast<int>(img.width * 3);
	int newHeight = static_cast<int>(img.height * 3);
	ImageResizeNN(&img, newWidth, newHeight);

	//PLayer Data
	Texture2D PlayerTexture = LoadTextureFromImage(img);
	UnloadImage(img);
	
	Rectangle playerRectangle;
	playerRectangle.width = PlayerTexture.width / 4;
	playerRectangle.height = PlayerTexture.height / 6;
	playerRectangle.x = 0;
	playerRectangle.y = 0;
	Vector2 playerPos;
	playerPos.x = windowWidth/2 - playerRectangle.width/2;
	playerPos.y = windowHeight - playerRectangle.height;
	int jumpForce = -20;
	int yVelocity = 0;
	bool isInAir = false;

	TraceLog(LOG_INFO, "PLAYER RECTANGLE WIDTH: %f", playerRectangle.width);
	TraceLog(LOG_INFO, "Player rectangle height: %f", playerRectangle.height);

	//Environment Data
	int floorPos = windowHeight - playerRectangle.height;
	const int gravityVelocity = 1;


	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
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
			yVelocity += gravityVelocity;
			isInAir = true;
		}

		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			yVelocity += jumpForce;
		}

		playerPos.y += yVelocity;

		DrawTextureRec(PlayerTexture, playerRectangle, playerPos, WHITE);

		EndDrawing();
	}
	UnloadTexture(PlayerTexture);
	CloseWindow();
	return 0;
}