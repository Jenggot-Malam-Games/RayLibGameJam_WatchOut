/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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

extern Texture2D instruction ; /// This is cover Texture

struct RadarObject
{
	struct GameObject line;
	struct GameObject triangle;
	struct GameObject dot;
	struct GameObject circle;
	struct GameObject circle2;
	
	int rotate;
	
	void (*RadarDraw)();
} radarObject;

struct Stage
{
	struct GameObject background;
	
	void (*Init)();
	void (*Update)();
	void (*Draw)();

};
struct StageManager
{
	struct Stage stage1;
	struct Stage stage2;
	struct Stage stage3;
	
	struct Stage* ptrStageToAct;
	
	struct GameObject screenEndingLose;
	struct GameObject screenEndingNice;
	struct GameObject screenEndingRayLib;
	
	struct GameObject day1;
	struct GameObject day2;
	struct GameObject dayRay;
	
	bool endingLose;
	bool endingNextDay;
	bool endingRayLib;
	
	int gameState; // 0 lose, 1 nice, 2 RayLib
	
	struct GameObject* ptrScreenEnding;
	
	void (*Init)();
	void (*Update)();
	void (*Draw)();
} stageManager;

struct BigAsteroid
{
	int count;
	int defence[ BIGASTEROIDCOUNT ];
	
	Vector2 speed[ BIGASTEROIDCOUNT ];
	
	int bigAsteroidToDraw[ BIGASTEROIDCOUNT ];
	
	struct GameObject * theBigAsteroids;
} bigAsteroid;

struct SmallAsteroid
{
	int count;
	int defence[ ASTEROIDCOUNT ];
	
	Vector2 speed[ ASTEROIDCOUNT ];
	
	int smallAsteroidToDraw[ ASTEROIDCOUNT ];
	
	struct GameObject * theAsteroids;
} smallAsteroid;

struct RayAsteroid
{
	int count;
	int defence[ 4 ];
	
	Vector2 speed[ 4 ];
	
	int rayAsteroidToDraw[ 4 ];
	
	struct GameObject * theRayAsteroids;
} rayAsteroid;

struct BigMissile
{
	int count;
	int damage;
	
	Vector2 speed[ BIGMISSILECOUNT ];
	
	int bigMissileToDraw[ BIGMISSILECOUNT ];

	struct GameObject * theBigMissiles;
} bigMissile;

struct SmallMissile
{
	int count;
	int damage;
	
	bool isSpeedLocked;
	Vector2 speed[ MISSILECOUNT ];
	
	Vector2 speedFactor;
	
	int smallMissileToDraw[ MISSILECOUNT ];
	
	struct GameObject * theMissiles;
} smallMissile;

struct Stage1Data
{
	int asteroidShowCurr;
	int asteroidShowCount;
} stage1Data;

struct Stage2Data
{
	int asteroidShowCurr;
	int asteroidShowCount;
} stage2Data;

struct Stage3Data
{
	int asteroidShowCurr;
	int asteroidShowCount;
} stage3Data;

/////
void Swap(int* s, int* t)
{
	int temp = *s;
	*s = *t;
	*t = temp;
}

/////
static float speedMul = 0.001f;

void ResetData(Vector2* thePos, Rectangle* theRect, Texture2D* theTexture)
{
	int val ;
	val = GetRandomValue(0, 3); // 0 = up, 1 = left, 2 = bottom, 3 = right
	if( val == 1)
	{
			
		thePos->x = GetRandomValue(-8, -4); 
		thePos->y = GetRandomValue(-8, GetScreenHeight() + 8); 
			
	}
	else if ( val == 0 )
	{
		thePos->y = GetRandomValue(-8, -4); 
		thePos->x = GetRandomValue( - 8, GetScreenWidth() + 8); 	
			
	}else if ( val == 2 )
	{
		thePos->y = GetRandomValue( GetScreenHeight() + 8, GetScreenHeight() + 4); 
		thePos->x = GetRandomValue( - 8, GetScreenHeight() + 8); 	
			
	}else if ( val == 3)
	{
		thePos->x = GetRandomValue( GetScreenWidth() + 8,  GetScreenWidth() + 4); 
		thePos->y = GetRandomValue( - 8, GetScreenWidth() + 8); 	
			
	}
	
	int xx = theTexture->width/2;
	int yy = theTexture->height/2;
	*theRect = (Rectangle){ thePos->x - xx, thePos->y - yy , 
								theTexture->width, theTexture->height };

}
void ResetAllData()
{
	for(int i = 0; i < ASTEROIDCOUNT; i++)
	{
		/// X Y Pos
		ResetData( &asteroids[i].position, &asteroids[i].rect, asteroids[i].ptrTexture);	
	}	
	
	for(int i = 0; i < BIGASTEROIDCOUNT; i++)
	{
		/// X Pos
		ResetData( &asteroidsBig[i].position, &asteroidsBig[i].rect, asteroidsBig[i].ptrTexture);
	}
	
	for(int i = 0; i < RAYASTEROIDCOUNT; i++)
	{
		/// X Pos
		ResetData( &raylibMonster[i].position, &raylibMonster[i].rect, raylibMonster[i].ptrTexture);
	}
	
	//// Set Speed of each Asteroid toward the middle
	
	for(int i = 0; i < ASTEROIDCOUNT; i++)
	{
		/// X Y speed	
		smallAsteroid.speed[i].x = (radarObject.circle.position.x - asteroids[i].position.x) * speedMul;
		smallAsteroid.speed[i].y = (radarObject.circle.position.y - asteroids[i].position.y) * speedMul;
	}	
	
	for(int i = 0; i < BIGASTEROIDCOUNT; i++)
	{
		/// X Y speed
		bigAsteroid.speed[i].x = (radarObject.circle.position.x - asteroidsBig[i].position.x) * speedMul;
		bigAsteroid.speed[i].y = (radarObject.circle.position.y - asteroidsBig[i].position.y ) * speedMul;
	}
	
	/// RayLIB monster
	for(int i = 0; i < 4; i++)
	{
		/// X Y speed
		rayAsteroid.speed[i].x = (radarObject.circle.position.x - raylibMonster[i].position.x) * speedMul;
		rayAsteroid.speed[i].y = (radarObject.circle.position.y - raylibMonster[i].position.y ) * speedMul;
	}
	
	
	//// Missile Position Set
	for( int i =0; i < MISSILECOUNT; i++)
	{
		missiles[i].position = radarObject.circle.position ;
	}
}

static float rudalMul = 0.5f;
bool RudalLaunch()
{
	// check if the pool has a missile
	if( smallMissile.count >= MISSILECOUNT )
		return false;

	Vector2 getMouse = GetMousePosition();
	
	float xElapsed = ( getMouse.x - radarObject.dot.position.x );
	float yElapsed = ( getMouse.y - radarObject.dot.position.y );
	
	smallMissile.count += 1;
	
	smallMissile.speed[ smallMissile.smallMissileToDraw[ smallMissile.count - 1 ] ].x = ( xElapsed /  smallMissile.speedFactor.x ) * rudalMul;	
	smallMissile.speed[ smallMissile.smallMissileToDraw[ smallMissile.count - 1 ] ].y = ( yElapsed /  smallMissile.speedFactor.y ) * rudalMul;	

	
	return true;
}

/////////////////////
void InitStage1()
{
	ResetAllData();
	
	smallMissile.count = 0;
	smallMissile.damage = 1;
	smallMissile.theMissiles =  &missiles[0];
	smallMissile.speedFactor = (Vector2){ 20.0f , 20.0f };
	for(int i = 0 ; i< MISSILECOUNT; i++)
		smallMissile.smallMissileToDraw[ i ] = i;
	
	
	bigAsteroid.count = 0;
	//bigAsteroid.speed = (Vector2){ 128 , 128 };
	bigAsteroid.theBigAsteroids = &asteroidsBig[0];
	for(int i = 0 ; i< BIGASTEROIDCOUNT; i++)
	{
		bigAsteroid.bigAsteroidToDraw[ i ] = i;
		bigAsteroid.defence[ i ] = 2;
	}
	
	
	smallAsteroid.count = 12;
	//smallAsteroid.speed = (Vector2){ 128 , 128 };
	smallAsteroid.theAsteroids = &asteroids[0];
	for(int i = 0 ; i< ASTEROIDCOUNT; i++)
	{
		smallAsteroid.smallAsteroidToDraw[ i ] = i;
		smallAsteroid.defence[ i ] = 1;
	}
	
	
	stage1Data.asteroidShowCount = smallAsteroid.count ;
	stage1Data.asteroidShowCurr = 0 ;
	radarObject.rotate = -1;
}

int counterRudal = 0;
void UpdateStage1()
{
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
	}	
	
	// missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].x;
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].y;
		
		// Destroy the missile if outside of the radar
		if( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x < 0 || 
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x > 256 ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y < 0   ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y > 256 )
				{
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;
					
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);
					
					// Destroy ? wkwkwwk
					smallMissile.count--;
				}
				
		// Check Collision Each Asteroid
			for(int j = 0; j< smallAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					smallAsteroid.defence[ j ]--;
					// still strong
					if( smallAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].position, &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 2
					smallAsteroid.defence[ j ] = 1;
					
					// swap  with last Asteroid before destroying
					Swap( &smallAsteroid.smallAsteroidToDraw[j] , &smallAsteroid.smallAsteroidToDraw[ smallAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					smallAsteroid.count--;
					
					
					stage1Data.asteroidShowCurr++;
					
				}
			}

								/// Won the stage
			if( stage1Data.asteroidShowCurr >= stage1Data.asteroidShowCount )
			{
				stage1Data.asteroidShowCurr = 0;
				
				// change to stage 2
				stageManager.ptrStageToAct = &stageManager.stage2;
				stageManager.ptrStageToAct->Init();
			}
	}
	
	if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
	{
		if(counterRudal == 0)
		{
			if( RudalLaunch() ) PlaySoundMulti(fxCoin);
			
			counterRudal = 15;
		}
	}
	counterRudal--;
	if(counterRudal < 0)
		counterRudal = 0;
	
	radarObject.rotate++;
	if( radarObject.rotate >= 360 )
	{
		radarObject.rotate = 0;
	}
	
}
void DrawStage1()
{
	DrawTextureExCenter(*radarObject.circle2.ptrTexture, radarObject.circle2.position, 0.0f, 1.0f, WHITE);
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		////DrawRectangleRec( smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect , WHITE );
		DrawTextureExCenter(*smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].ptrTexture, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}	
	
	// Missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		DrawTextureExCenter(*smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].ptrTexture, smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	/// Draw Radar Design
	
	DrawTextureEx(*radarObject.line.ptrTexture, radarObject.line.position, radarObject.rotate, 1.0f, WHITE);
	
	DrawTextureExCenter(*radarObject.circle.ptrTexture, radarObject.circle.position, 0.0f, 1.0f, WHITE);
	
	//DrawRectangleRec( radarObject.dot.rect , WHITE );
	DrawTextureExCenter(*radarObject.dot.ptrTexture, radarObject.dot.position, 0.0f, 1.0f, WHITE);
}

//////////////////////
void InitStage2()
{
	ResetAllData();
	
	smallMissile.count = 0;
	smallMissile.damage = 1;
	smallMissile.theMissiles =  &missiles[0];
	smallMissile.speedFactor = (Vector2){ 20.0f , 20.0f };
	for(int i = 0 ; i< MISSILECOUNT; i++)
		smallMissile.smallMissileToDraw[ i ] = i;
	
	
	bigAsteroid.count = 3;
	//bigAsteroid.speed = (Vector2){ 128 , 128 };
	bigAsteroid.theBigAsteroids = &asteroidsBig[0];
	for(int i = 0 ; i< BIGASTEROIDCOUNT; i++)
	{
		bigAsteroid.bigAsteroidToDraw[ i ] = i;
		bigAsteroid.defence[ i ] = 3;
	}
	
	
	smallAsteroid.count = 8;
	//smallAsteroid.speed = (Vector2){ 128 , 128 };
	smallAsteroid.theAsteroids = &asteroids[0];
	for(int i = 0 ; i< ASTEROIDCOUNT; i++)
	{
		smallAsteroid.smallAsteroidToDraw[ i ] = i;
		smallAsteroid.defence[ i ] = 1;
	}
	
	stage2Data.asteroidShowCount = smallAsteroid.count + bigAsteroid.count;
	
	radarObject.rotate = -1;
}
void UpdateStage2()
{
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
	}	
	//bigAsteroid
	for(int i = 0; i< bigAsteroid.count; i++)
	{
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position.x += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].x;
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position.y += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].rect.x += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].x;
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].rect.y += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].y;
	}
	
	// missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].x;
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].y;
		
		// Destroy the missile if outside of the radar
		if( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x < 0 || 
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x > 256 ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y < 0   ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y > 256 )
				{
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;
					
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);
					
					// Destroy ? wkwkwwk
					smallMissile.count--;
				}
				
		// Check Collision Each Asteroid
			for(int j = 0; j< smallAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					smallAsteroid.defence[ j ]--;
					// still strong
					if( smallAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].position, &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 2
					smallAsteroid.defence[ j ] = 1;
					
					// swap  with last Asteroid before destroying
					Swap( &smallAsteroid.smallAsteroidToDraw[j] , &smallAsteroid.smallAsteroidToDraw[ smallAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					smallAsteroid.count--;
					
					
					stage2Data.asteroidShowCurr++;
				}
			}	
			
			// Big Asteroid
			for(int j = 0; j< bigAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					bigAsteroid.defence[ j ]--;
					// still strong
					if( bigAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].position, &bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].rect, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 2
					bigAsteroid.defence[ j ] = 3;
					
					// swap  with last Asteroid before destroying
					Swap( &bigAsteroid.bigAsteroidToDraw[j] , &bigAsteroid.bigAsteroidToDraw[ bigAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					bigAsteroid.count--;
					
					
					stage2Data.asteroidShowCurr++;
				}
			}	
		
							/// Won the stage
			if( stage2Data.asteroidShowCurr >= stage2Data.asteroidShowCount )
			{
				stage2Data.asteroidShowCurr = 0;
				
				// change to stage 2
				stageManager.ptrStageToAct = &stageManager.stage3;
				stageManager.ptrStageToAct->Init();
			}
	}
	
	if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
	{
		if(counterRudal == 0)
		{
			if( RudalLaunch() ) PlaySoundMulti(fxCoin);
			
			counterRudal = 15;
		}
	}
	counterRudal--;
	if(counterRudal < 0)
		counterRudal = 0;
	
	radarObject.rotate++;
	if( radarObject.rotate >= 360 )
	{
		radarObject.rotate = 0;
	}
}
void DrawStage2()
{
	DrawTextureExCenter(*radarObject.circle2.ptrTexture, radarObject.circle2.position, 0.0f, 1.0f, WHITE);
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		////DrawRectangleRec( smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect , WHITE );
		DrawTextureExCenter(*smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].ptrTexture, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}	
	//bigAsteroid
	for(int i = 0; i< bigAsteroid.count; i++)
	{
		//DrawRectangleRec( bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].rect , WHITE );
		DrawTextureExCenter(*bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].ptrTexture, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	// Missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		DrawTextureExCenter(*smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].ptrTexture, smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	/// Draw Radar Design
	
	DrawTextureEx(*radarObject.line.ptrTexture, radarObject.line.position, radarObject.rotate, 1.0f, WHITE);
	
	DrawTextureExCenter(*radarObject.circle.ptrTexture, radarObject.circle.position, 0.0f, 1.0f, WHITE);
	
	//DrawRectangleRec( radarObject.dot.rect , WHITE );
	DrawTextureExCenter(*radarObject.dot.ptrTexture, radarObject.dot.position, 0.0f, 1.0f, WHITE);
}

//////////////////////
void InitStage3()
{
	ResetAllData();
	
	smallMissile.count = 0;
	smallMissile.damage = 1;
	smallMissile.theMissiles =  &missiles[0];
	smallMissile.speedFactor = (Vector2){ 20.0f , 20.0f };
	for(int i = 0 ; i< MISSILECOUNT; i++)
		smallMissile.smallMissileToDraw[ i ] = i;
	
	
	bigAsteroid.count = 5;
	//bigAsteroid.speed = (Vector2){ 128 , 128 };
	bigAsteroid.theBigAsteroids = &asteroidsBig[0];
	for(int i = 0 ; i< BIGASTEROIDCOUNT; i++)
	{
		bigAsteroid.bigAsteroidToDraw[ i ] = i;
		bigAsteroid.defence[ i ] = 2;
	}
	
	
	smallAsteroid.count = 4;
	//smallAsteroid.speed = (Vector2){ 128 , 128 };
	smallAsteroid.theAsteroids = &asteroids[0];
	for(int i = 0 ; i< ASTEROIDCOUNT; i++)
	{
		smallAsteroid.smallAsteroidToDraw[ i ] = i;
		smallAsteroid.defence[ i ] = 1;
	}
	
	rayAsteroid.count = RAYASTEROIDCOUNT;
	rayAsteroid.theRayAsteroids = &raylibMonster[0];
	for(int i = 0 ; i< RAYASTEROIDCOUNT; i++)
	{
		rayAsteroid.rayAsteroidToDraw[ i ] = i;
		rayAsteroid.defence[ i ] = 9999; /// Uwow
	}
	
	stage3Data.asteroidShowCount = smallAsteroid.count + bigAsteroid.count + rayAsteroid.count;
	
	radarObject.rotate = -1;
}
void UpdateStage3()
{
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.x += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].x;
		smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect.y += smallAsteroid.speed[ smallAsteroid.smallAsteroidToDraw[i] ].y;
		
	}	
	//bigAsteroid
	for(int i = 0; i< bigAsteroid.count; i++)
	{
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position.x += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].x;
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position.y += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].rect.x += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].x;
		bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].rect.y += bigAsteroid.speed[ bigAsteroid.bigAsteroidToDraw[i] ].y;
	}
	
	//rayAsteroid
	for(int i = 0; i< rayAsteroid.count; i++)
	{
		rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].position.x += rayAsteroid.speed[ rayAsteroid.rayAsteroidToDraw[i] ].x;
		rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].position.y += rayAsteroid.speed[ rayAsteroid.rayAsteroidToDraw[i] ].y;
		
		/// Update Rectangle
		rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].rect.x += rayAsteroid.speed[ rayAsteroid.rayAsteroidToDraw[i] ].x;
		rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].rect.y += rayAsteroid.speed[ rayAsteroid.rayAsteroidToDraw[i] ].y;
	}
	
	// missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].x;
		smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y += smallMissile.speed[ smallMissile.smallMissileToDraw[i] ].y;
		
		// Destroy the missile if outside of the radar
		if( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x < 0 || 
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.x > 256 ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y < 0   ||
				smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position.y > 256 )
				{
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;
					
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);
					
					// Destroy ? wkwkwwk
					smallMissile.count--;
				}
				
		// Check Collision Each Asteroid
			for(int j = 0; j< smallAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					smallAsteroid.defence[ j ]--;
					// still strong
					if( smallAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].position, &smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].rect, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 2
					smallAsteroid.defence[ j ] = 1;
					
					// swap  with last Asteroid before destroying
					Swap( &smallAsteroid.smallAsteroidToDraw[j] , &smallAsteroid.smallAsteroidToDraw[ smallAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					smallAsteroid.count--;
				}
			}	
			
			// Big Asteroid
			for(int j = 0; j< bigAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					bigAsteroid.defence[ j ]--;
					// still strong
					if( bigAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].position, &bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].rect, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 2
					bigAsteroid.defence[ j ] = 3;
					
					// swap  with last Asteroid before destroying
					Swap( &bigAsteroid.bigAsteroidToDraw[j] , &bigAsteroid.bigAsteroidToDraw[ bigAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					bigAsteroid.count--;
				}
			}	
			
			// Ray Asteroid
			for(int j = 0; j< rayAsteroid.count; j++)
			{
				if( smallMissile.count <= 0)
					break;
				
				if ( CheckCollisionPointRec( smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[j] ].rect ) )
				{
					PlaySound(fxExplode);
					// DESTROY RUDAL
					// Reset the destroyed rudal to normal position
					smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position = radarObject.circle.position;	
					// swap  with last missile before destroying
					Swap( &smallMissile.smallMissileToDraw[i] , &smallMissile.smallMissileToDraw[ smallMissile.count - 1]);	
					// Destroy ? wkwkwwk
					smallMissile.count--;
					
					/////////////////////////////////////////////////
					rayAsteroid.defence[ j ]--;
					// still strong
					if( rayAsteroid.defence[ j ] > 0 )
						continue;

					// DESTROY ASTEROID
					// Reset Asteroid
					ResetData( &rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[j] ].position, &rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[j] ].rect, rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[j] ].ptrTexture);
					// set back defense to 
					rayAsteroid.defence[ j ] = 9999;
					
					// swap  with last Asteroid before destroying
					Swap( &rayAsteroid.rayAsteroidToDraw[j] , &rayAsteroid.rayAsteroidToDraw[ rayAsteroid.count - 1]);
					
					// Destroy ? wkwkwwk
					rayAsteroid.count--;
				}
			}	
		
	}
	
	if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
	{
		if(counterRudal == 0)
		{
			if( RudalLaunch() ) PlaySoundMulti(fxCoin);
			
			counterRudal = 15;
		}
	}
	counterRudal--;
	if(counterRudal < 0)
		counterRudal = 0;
	
	radarObject.rotate++;
	if( radarObject.rotate >= 360 )
	{
		radarObject.rotate = 0;
	}
}
void DrawStage3()
{
	DrawTextureExCenter(*radarObject.circle2.ptrTexture, radarObject.circle2.position, 0.0f, 1.0f, WHITE);
	//smallAsteroid
	for(int i = 0; i< smallAsteroid.count; i++)
	{
		////DrawRectangleRec( smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].rect , WHITE );
		DrawTextureExCenter(*smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].ptrTexture, smallAsteroid.theAsteroids[ smallAsteroid.smallAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}	
	//bigAsteroid
	for(int i = 0; i< bigAsteroid.count; i++)
	{
		DrawTextureExCenter(*bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].ptrTexture, bigAsteroid.theBigAsteroids[ bigAsteroid.bigAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	//rayAsteroid
	for(int i = 0; i< rayAsteroid.count; i++)
	{
		DrawTextureExCenter(*rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].ptrTexture, rayAsteroid.theRayAsteroids[ rayAsteroid.rayAsteroidToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	// Missile
	for(int i = 0; i< smallMissile.count; i++)
	{
		DrawTextureExCenter(*smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].ptrTexture, smallMissile.theMissiles[ smallMissile.smallMissileToDraw[i] ].position, 0.0f, 1.0f, WHITE);
	}
	
	/// Draw Radar Design
	
	DrawTextureEx(*radarObject.line.ptrTexture, radarObject.line.position, radarObject.rotate, 1.0f, WHITE);
	
	DrawTextureExCenter(*radarObject.circle.ptrTexture, radarObject.circle.position, 0.0f, 1.0f, WHITE);
	
	////DrawRectangleRec( radarObject.dot.rect , WHITE );
	DrawTextureExCenter(*radarObject.dot.ptrTexture, radarObject.dot.position, 0.0f, 1.0f, WHITE);
}

//////////////////////////////////
void InitStageManager()
{

	radarObject.dot.ptrTexture = &textureData[4];
	radarObject.dot.position = (Vector2){ GetScreenWidth()/2 , GetScreenHeight()/2 };
	
	int xx = radarObject.dot.ptrTexture->width/2;
	int yy = radarObject.dot.ptrTexture->height/2;
	radarObject.dot.rect = (Rectangle){ radarObject.dot.position.x - xx, radarObject.dot.position.y - yy , 
								radarObject.dot.ptrTexture->width, radarObject.dot.ptrTexture->height };
	
	
	radarObject.circle.ptrTexture = &textureData[5];
	radarObject.circle.position = (Vector2){ GetScreenWidth()/2 , GetScreenHeight()/2 };
	
	radarObject.circle2.ptrTexture = &textureData[6];
	radarObject.circle2.position = (Vector2){ GetScreenWidth()/2 , GetScreenHeight()/2 };
	
	radarObject.line.ptrTexture = &textureData[7];
	radarObject.line.position = (Vector2){ GetScreenWidth()/2 , GetScreenHeight()/2 };
	
	
	
	stageManager.stage1.Init = InitStage1;
	stageManager.stage1.Update = UpdateStage1;
	stageManager.stage1.Draw = DrawStage1;
		
	stageManager.stage2.Init = InitStage2;
	stageManager.stage2.Update = UpdateStage2;
	stageManager.stage2.Draw = DrawStage2;
		
	stageManager.stage3.Init = InitStage3;
	stageManager.stage3.Update = UpdateStage3;
	stageManager.stage3.Draw = DrawStage3;
	
	//CheckCollisionPointCircle
	stageManager.day1.ptrTexture = &textureData[8];
	stageManager.day2.ptrTexture = &textureData[9];
	stageManager.dayRay.ptrTexture = &textureData[10];
	
	////////////////////
	//stageManager.stage1.Init();
/* 	stageManager.stage2.Init();
	stageManager.stage3.Init(); */
	
	stageManager.ptrStageToAct = &stageManager.stage1;
	stageManager.ptrStageToAct->Init();
	
}
void UpdateStageManager()
{
	stageManager.ptrStageToAct->Update();
}
void DrawStageManager()
{
	if(stageManager.endingLose)
	{
		
	}
	if(stageManager.endingNextDay)
	{
		
	}
	if(stageManager.endingRayLib)
	{
		
	}
	
	stageManager.ptrStageToAct->Draw();
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
	
	stageManager.Init = InitStageManager;
	stageManager.Update = UpdateStageManager;
	stageManager.Draw = DrawStageManager;
	
	stageManager.endingLose = false;
	stageManager.endingNextDay = false;
	stageManager.endingRayLib = false;
	
	stageManager.Init();
	
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    stageManager.Update();

    // Press enter or tap to change to ENDING screen
/*     if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    } */
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
	
	stageManager.Draw();
    // TODO: Draw GAMEPLAY screen here!
/*     DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    Vector2 pos = { 20, 10 };
    DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize*3.0f, 4, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON); */
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}