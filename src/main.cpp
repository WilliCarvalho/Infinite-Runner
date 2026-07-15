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


	
	//PLayer Data
	Texture2D PlayerTexture = LoadTexture("2DTexturesCharacter/CharacterSheet.png");
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