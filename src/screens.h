/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
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

#ifndef SCREENS_H
#define SCREENS_H

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;
extern Sound fxExplode;
extern Music fxRayAudio;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

#ifdef __cplusplus
}
#endif

// My Custom
extern void DrawTextureExCenter(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);

struct GameObject
{
	Vector2 position;
	Rectangle rect;
	
	Texture2D* ptrTexture;
};


#define TEXTUREDATACOUNT 20

#define ASTEROIDCOUNT 20
#define BIGASTEROIDCOUNT 20

#define MISSILECOUNT 30
#define BIGMISSILECOUNT 50


#define RAYASTEROIDCOUNT 4


// Load once
extern Texture2D textureData[TEXTUREDATACOUNT];
// 0 asteroid
// 1 missile


extern struct GameObject asteroids[ASTEROIDCOUNT];
extern struct GameObject missiles[MISSILECOUNT];

extern struct GameObject asteroidsBig[BIGASTEROIDCOUNT];
extern struct GameObject missilesBig[BIGMISSILECOUNT];


extern struct GameObject raylibMonster[4];

extern int winState; // 0 lose, 1 raylib

/* extern struct GameObject boss1[5];
extern struct GameObject bossRay[50];
 */
/* #define RMEM_IMPLEMENTATION
#include "extras/rmem.h"
 */
#define EASINGS_STATIC_INLINE
#include "extras/easings.h"

#endif // SCREENS_H