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

	//rectangle dimentions
	const int width = 50;
	const int height = 80;

	int posY = windowHeight - height;
	int jumpForce = -10;
	int gravityVelocity = 10;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(GREEN);

		if (IsKeyDown(KEY_SPACE))
		{
			posY += jumpForce;
		}
		else if(posY <= windowHeight - height)
		{
			posY += gravityVelocity;
		}

		DrawRectangle(windowWidth / 2, posY, width, height, YELLOW);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}












//int main ()
//{
//	// Tell the window to use vsync and work on high DPI displays
//	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
//
//	// Create the window and OpenGL context
//	InitWindow(800, 600, "Hello Raylib");
//
//	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
//	SearchAndSetResourceDir("resources");
//
//	// Load a texture from the resources directory
//	Texture wabbit = LoadTexture("wabbit_alpha.png");
//	
//	// game loop
//	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
//	{
//		// drawing
//		BeginDrawing();
//
//		// Setup the back buffer for drawing (clear color and depth buffers)
//		ClearBackground(BLACK);
//
//		// draw some text using the default font
//		DrawText("Hello Raylib", 200,200,20,WHITE);
//
//		// draw our texture to the screen
//		DrawTexture(wabbit, 400, 200, WHITE);
//		
//		// end the frame and get ready for the next one  (display frame, poll input, etc...)
//		EndDrawing();
//	}
//
//	// cleanup
//	// unload our texture so it can be cleaned up
//	UnloadTexture(wabbit);
//
//	// destroy the window and cleanup the OpenGL context
//	CloseWindow();
//	return 0;
//}
