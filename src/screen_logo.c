/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Logo Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int logoPositionX = 0;
static int logoPositionY = 0;

static int lettersCount = 0;

static int topSideRecWidth = 0;
static int leftSideRecHeight = 0;

static int bottomSideRecWidth = 0;
static int rightSideRecHeight = 0;

static int state = 0;              // Logo animation states
static float alpha = 1.0f;         // Useful for fading

///////////////////////////////
static int logoState = 0; // 0 is RayLib, 1 is JenggotMalam
Texture2D jenggotLogo ;
Texture2D jenggotType ;

/////////////
Texture2D cover ; ///////////////////// COVERRR
Texture2D instruction ; //////// space
/////////////

Vector2 logoPos;
Vector2 typePos;
static float theRot;


static int currentTime = 0;
static int duration = 75;
static float startPositionX = 0.0f;
float finalPositionX;


static float easeData = 0.0f;
//float currentPositionX = startPositionX;
static inline void MyEaseBackInOut( float* currentPositionX )
{
	if(currentTime >= duration)
	{
		return;
	}
	
   *currentPositionX = EaseBackInOut(currentTime, 0.0f, 1.0f, duration);
   currentTime++;
}

// My Custom
void DrawTextureExCenter(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
{
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, (float)texture.width*scale, (float)texture.height*scale };
    Vector2 origin = { (float)texture.width *0.5f * scale , (float)texture.height *0.5f * scale};

    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}


///////////////////// ONCE
Texture2D textureData[TEXTUREDATACOUNT];
struct GameObject asteroids[ASTEROIDCOUNT];
struct GameObject missiles[MISSILECOUNT];

struct GameObject asteroidsBig[BIGASTEROIDCOUNT];
struct GameObject missilesBig[BIGMISSILECOUNT];
/////////////////////////

//
//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

// Logo Screen Initialization logic
void InitLogoScreen(void)
{
    finishScreen = 0;
    framesCounter = 0;
    lettersCount = 0;

    logoPositionX = GetScreenWidth()/2 - 128;
    logoPositionY = GetScreenHeight()/2 - 128;

    topSideRecWidth = 16;
    leftSideRecHeight = 16;
    bottomSideRecWidth = 16;
    rightSideRecHeight = 16;

    state = 0;
    alpha = 1.0f;
	
	jenggotLogo = LoadTexture("resources/jenggotlogo.png");
	jenggotType = LoadTexture("resources/jenggottype.png");
	
	cover = LoadTexture("resources/cover.png");
	instruction = LoadTexture("resources/space.png");
	
	logoPos = (Vector2){50 + ((float)jenggotLogo.width *0.5f), 5 + ((float)jenggotLogo.height *0.5f) };
	//logoPos = (Vector2){0, 0 };
	typePos = (Vector2){ 128 , 256 - 31 };
	theRot = 0.0f;
	
	finalPositionX = 50 + ((float)jenggotLogo.width *0.5f);
	
	/////////////////////// ONCE
	//////////////////////

	textureData[0] = LoadTexture("resources/asteroid.png");
	textureData[1] = LoadTexture("resources/asteroid_big.png");
	textureData[2] = LoadTexture("resources/big_missile.png");
	textureData[3] = LoadTexture("resources/missile.png");
	
	
	textureData[4] = LoadTexture("resources/dot.png");
	textureData[5] = LoadTexture("resources/bgcircle.png");
	textureData[6] = LoadTexture("resources/bgcircle2.png");
	

	textureData[7] = LoadTexture("resources/line.png");

	/////
	
	for(int i =0; i < 20; i++)
	{
		asteroids[i].ptrTexture = &textureData[0]; // 0 Asteroid
	}
	
	for(int i =0; i < 50; i++)
	{
		missiles[i].ptrTexture = &textureData[3]; // 3 missile
	}
	
	for(int i =0; i < 20; i++)
	{
		asteroidsBig[i].ptrTexture = &textureData[1]; // 1 Asteroid
	}
	
	for(int i =0; i < 50; i++)
	{
		missilesBig[i].ptrTexture = &textureData[2]; // 2 missile
	}
	
	
	/////////////////////
}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{
	switch(logoState)
	{
		case 0:
		if (state == 0)                 // State 0: Top-left square corner blink logic
		{
			framesCounter++;

			if (framesCounter == 80)
			{
				state = 1;
				framesCounter = 0;      // Reset counter... will be used later...
			}
		}
		else if (state == 1)            // State 1: Bars animation logic: top and left
		{
			topSideRecWidth += 8;
			leftSideRecHeight += 8;

			if (topSideRecWidth == 256) state = 2;
		}
		else if (state == 2)            // State 2: Bars animation logic: bottom and right
		{
			bottomSideRecWidth += 8;
			rightSideRecHeight += 8;

			if (bottomSideRecWidth == 256) state = 3;
		}
		else if (state == 3)            // State 3: "raylib" text-write animation logic
		{
			framesCounter++;

			if (lettersCount < 10)
			{
				if (framesCounter/12)   // Every 12 frames, one more letter!
				{
					lettersCount++;
					framesCounter = 0;
				}
			}
			else    // When all letters have appeared, just fade out everything
			{
				if (framesCounter > 200)
				{
					alpha -= 0.02f;

					if (alpha <= 0.0f)
					{
						alpha = 0.0f;
						framesCounter = 0;
						logoState = 1;
						//finishScreen = 1;   // Jump to next screen
					}
				}
			}
		}
		break;
		
		
		case 1:
			//framesCounter++;
			theRot += 2.0f;
			
			MyEaseBackInOut(&easeData);
			if(theRot > 360.0f)
			{
				theRot = 0.0f;
				framesCounter = 0;
				finishScreen = 1;
			}
		break;
		
	}
}

// Logo Screen Draw logic
void DrawLogoScreen(void)
{
	switch(logoState)
	{
		case 0:
			if (state == 0)         // Draw blinking top-left square corner
			{
				if ((framesCounter/10)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
			}
			else if (state == 1)    // Draw bars animation: top and left
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
				DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
			}
			else if (state == 2)    // Draw bars animation: bottom and right
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
				DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

				DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
				DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
			}
			else if (state == 3)    // Draw "raylib" text-write animation + "powered by"
			{
				DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
				DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

				DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
				DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

				DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

				DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));

				if (framesCounter > 20) DrawText("powered by", logoPositionX, logoPositionY - 27, 20, Fade(DARKGRAY, alpha));
			}
			
		break;
		
		//void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
		case 1:
			DrawTextureExCenter(jenggotLogo, logoPos, theRot, 1.0f*easeData, WHITE);
			DrawTextureExCenter(jenggotType, typePos, 0.0f, 1.0f, WHITE);
		break;											
	}
}

// Logo Screen Unload logic
void UnloadLogoScreen(void)
{
    // Unload LOGO screen variables here!
}

// Logo Screen should finish?
int FinishLogoScreen(void)
{
    return finishScreen;
}
