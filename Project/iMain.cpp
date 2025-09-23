#define _CRT_SECURE_NO_WARNINGS
#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <direct.h>

// Mouse position for hover detection
int mouseX = -1, mouseY = -1;

// Game state control variables
int FirstPage1 = 1;
int StartPage1 = 0;
int StartPage2 = 0;
int L_FirstPage = 0;
int SecondPage = 0;
int ThirdPage = 0;
int AboutPage = 0;
int GameOverPage = 0;
int AnimationCounter = 0;
int score = 0;
int score1 = 0;
int highestScore;  // globally
int scoreLevel2;
int lives = 5;
int LevelUpPage = 0;
int gameOver = 0;
int youWin = 0;
int showSettingsMenu = 0;
int soundOn = 1;
int HowToPlayPage = 0;
int ScoresPage = 0;
int hopTimer = 0;
int isInvisible = 0;       // rabbit invisible kina
int invisibleTimer = 0; // kotokkhon thakbe invisible
int currentFrame = 0;        // kon frame show hobe
int rabbitDirection = 0;
int rabbitWidth = 122;
int rabbitHeight = 125;
int screenWidth = 966;
int screenHeight = 743;
int hopDuration = 8 * 1000 / 30;
bool isHopping = false;
bool musicon = 1;

// Rabbit position and animation state
int x = 300, y = 300;
char position = 's';

//  Rabbit sprite animation frames
int rabbitFrameIndex = 0;
int rabbitTotalFrames = 2;

int squirrelFrameIndex = 0;
int squirrelTotalFrames = 6; // 6 frames per direction

char *squirrelRightFrames[6] = {
	"Images\\squirrelRight1.bmp",
	"Images\\squirrelRight2.bmp",
	"Images\\squirrelRight3.bmp",
	"Images\\squirrelRight4.bmp",
	"Images\\squirrelRight5.bmp",
	"Images\\squirrelRight6.bmp"
};

char *squirrelLeftFrames[6] = {
	"Images\\squirrelLeft1.bmp",
	"Images\\squirrelLeft2.bmp",
	"Images\\squirrelLeft3.bmp",
	"Images\\squirrelLeft4.bmp",
	"Images\\squirrelLeft5.bmp",
	"Images\\squirrelLeft6.bmp"
};



char *rabbitRightFrames[5] = {
	"Images\\rabbitright1.bmp",
	"Images\\rabbitright2.bmp",
	"Images\\rabbitright3.bmp",
	"Images\\rabbitright4.bmp",
	"Images\\rabbitright5.bmp"
};
char *rabbitLeftFrames[5] = {
	"Images\\rabbitleft1.bmp",
	"Images\\rabbitleft2.bmp",
	"Images\\rabbitleft3.bmp",
	"Images\\rabbitleft4.bmp",
	"Images\\rabbitleft5.bmp"
};
char *rabbitFrontFrames[4] = {
	"Images\\rabbitfront1.bmp",
	"Images\\rabbitfront2.bmp",
	"Images\\rabbitfront3.bmp",
	"Images\\rabbitfront4.bmp"
};

char *rabbitBackFrames[3] = {
	"Images\\rabbitback1.bmp",
	"Images\\rabbitback2.bmp",
	"Images\\rabbitback3.bmp"
};

struct Trap {
	int x, y;      // position
	int width;     // trap width
	int height;    // trap height
};

// Example trap array
Trap traps[] = {
	{ 100, 310, 50, 50 },
	{ 380, 120, 50, 50 },
	{ 200, 410, 50, 50 },
	{ 480, 220, 50, 50 },
	{ 500, 610, 50, 50 },
	{ 780, 220, 50, 50 }
};
int trapCount = sizeof(traps) / sizeof(traps[0]);

struct Food {
	int x, y;
	int visible;
	char *imagePath;
	int respawnTimer;
};

#define MAX_STRAWBERRIES 3

struct Strawberry {
	int x, y;
	int visible;
	int respawnTimer;
};

Strawberry strawberries[MAX_STRAWBERRIES];


int overlapsWithTrap(int fx, int fy, int fw, int fh,
	int tx, int ty, int tw, int th) {
	return !(fx + fw < tx || fx > tx + tw ||
		fy + fh < ty || fy > ty + th);
}



void spawnFoodSafely(Food &f, int screenWidth, int screenHeight, int foodWidth, int foodHeight) {
	int safe = 0;
	while (!safe) {
		f.x = rand() % (screenWidth - foodWidth);
		f.y = rand() % (screenHeight - foodHeight);

		safe = 1; // assume it's safe
		for (int i = 0; i < trapCount; i++) {
			if (overlapsWithTrap(f.x, f.y, foodWidth, foodHeight,
				traps[i].x, traps[i].y, traps[i].width, traps[i].height)) {
				safe = 0;
				break;
			}
		}
	}
}

// Updated respawnFood function
void respawnFood(Food &f) {
	spawnFoodSafely(f, 900, 700, 50, 50); // adjust screen and food size
	f.visible = 1;
}

// Function Declarations
void First_Page();
void Start_Page();
void DrawStartPage();
void Level_1FirstPage();
void Level_2FirstPage();
void Level_3FirstPage();
void About_Page();
void LevelUpConfirmationPage();
void GameOver_Page();
void DrawLevel1Background();
void generateRandomPosition(int &rx, int &ry);
void respawnFood(Food &f);
void saveScores();
void loadScores();

void NameButtonClickHandler();
void StartButtonClickHandler();
void Level_1Handle();
void Level_2Handle();
void Level_3Handle();
void BackButtonClickHandler();
void BackButtonClickHandler2();
void AboutPageClickHandler();
void PlayButtonClickHandler();
#define MAX_FOODS 20



Food carrots[] = {
	{ 500, 500, 1, "Images\\carrot.bmp" },
	{ 400, 300, 1, "Images\\carrot.bmp" },
	{ 300, 50, 1, "Images\\carrot.bmp" },
	{ 600, 200, 1, "Images\\carrot.bmp" },
	{ 300, 480, 1, "Images\\carrot.bmp" },
	{ 700, 280, 1, "Images\\carrot.bmp" }
};
int totalCarrots = 6;

Food grapes[] = {
	{ 470, 50, 1, "Images\\grapes.bmp" },
	{ 200, 250, 1, "Images\\grapes.bmp" },
	{ 400, 450, 1, "Images\\grapes.bmp" }
};
int totalGrapes = 3;

Food apples[] = {
	{ 550, 350, 1, "Images\\apple.bmp" },
	{ 180, 530, 1, "Images\\apple.bmp" }
};
int totalApples = 2;

struct Hawk {
	int x, y;
	int visible;
	int active;
	int speed;
	int direction;
} hawk;
struct Squirrel {
	int x, y;
	int visible;
	int active;
	int speed;
	int direction;
} squirrel;

int hawkTimer = 0;
int hawkSpawnInterval = 55;

int squirrelTimer = 0;
int squirrelSpawnInterval = 66;

void initHawk() {
	hawk.visible = 0;
	hawk.speed = 1;
}
void initSquirrel() {
	squirrel.visible = 0;
	squirrel.speed = 4;
}
void checkGameOver() {
	if (lives <= 0){
		lives = 0;
		if (!gameOver) { 
			gameOver = 1;

			if (score > highestScore) {
				highestScore = score;
				//saveHighScore();
			}
			if (score1 >scoreLevel2) {
				scoreLevel2 = score1;
				//saveHighScore();
			}

			if (soundOn){
				PlaySound("Audios\\Gameover.wav", NULL, SND_ASYNC); // <-- Game Over sound
			}
			else{
				PlaySound(0, 0, 0);
			}
		}
		saveScores();
		gameOver = 1;
		L_FirstPage = 0;
		SecondPage = 0;
		ThirdPage = 0;
		GameOverPage = 1;
	}
}

bool isMouseOver(int x1, int y1, int x2, int y2) {
	return (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2);
}

void GameOver_Page() {
	iShowBMP(0, 0, "Images\\Game_Over.bmp");

	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);

	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}
int hawkWidth = 80;   // Hawk sprite width
int hawkHeight = 80;  // Hawk sprite height

void spawnHawk() {
	hawk.visible = 1;
	hawk.active = 1;
	hawk.speed = 4;
	int screenWidth = 966;
	int screenHeight = 743;

	bool rabbitRight = x >= screenWidth / 2;
	bool rabbitTop = y >= screenHeight / 2;

	int validDirs[2];
	int count = 0;

	if (!rabbitRight) validDirs[count++] = 0; // right spawn allowed
	if (!rabbitTop) validDirs[count++] = 1;   // top spawn allowed

	if (count == 0) {
		hawk.x = screenWidth - hawkWidth;
		hawk.y = screenHeight - hawkHeight;
		hawk.direction = rand() % 2;
		return;
	}

	int dir = validDirs[rand() % count];

	if (dir == 0) { // right
		hawk.x = screenWidth - hawkWidth; // adjust to fit fully
		hawk.y = rand() % (screenHeight - hawkHeight);
		hawk.direction = 0; // move left
	}
	else if (dir == 1) { // top
		hawk.x = rand() % (screenWidth - hawkWidth);
		hawk.y = screenHeight - hawkHeight; // adjust top
		hawk.direction = 1; // move down
	}
}

void spawnSquirrel() {
	squirrel.visible = 1;
	squirrel.active = 1;
	squirrel.direction = rand() % 2;

	if (squirrel.direction == 0) { // left to right
		squirrel.x = -50;
		squirrel.y = rand() % 600 + 100;
	}
	else if (squirrel.direction == 1) { // top to bottom
		squirrel.x = 1000;
		squirrel.y = rand() % 600 + 100;
	}
}


// Move hawk + collision
void updateHawk() {
	if (!hawk.active) return;

	switch (hawk.direction) {

	case 0: hawk.x -= hawk.speed; break; // right → left
	case 1: hawk.y -= hawk.speed; break; // top → bottom

	}
	// Screen-এর বাইরে গেলে reset
	if (hawk.x < -50 || hawk.x > 1000 || hawk.y < -50 || hawk.y > 800) {
		hawk.active = 0;
		hawk.visible = 0;
		hawkTimer = 0;
	}


	// Collision rabbit-এর সাথে
	if (abs(hawk.x - x) < 60 && abs(hawk.y - y) < 60) {
		lives--;
		checkGameOver();
		hawk.visible = 0;
		hawk.active = 0;
		hawkTimer = 0;

		if (L_FirstPage == 1) {
			lives = 0;
			gameOver = 1;
		}
		else if (SecondPage == 1) {
			lives = 0;
			gameOver = 1; // Level 2 e game over
		}



		return;
	}
}

void updateSquirrel() {
	if (!squirrel.active) return;

	// Movement
	if (squirrel.direction == 0) squirrel.x += squirrel.speed;
	else if (squirrel.direction == 1) squirrel.x -= squirrel.speed;

	// Screen থেকে বাইরে গেলে
	if (squirrel.x < -50 || squirrel.x > 1000 || squirrel.y < -50) {
		squirrel.active = 0;
		squirrel.visible = 0;
		squirrelTimer = 0;
	}

	// খাবার খেয়ে ফেলে
	for (int i = 0; i < totalCarrots; i++) {
		if (carrots[i].visible && abs(squirrel.x - carrots[i].x) < 50 && abs(squirrel.y - carrots[i].y) < 50) {
			carrots[i].visible = 0;  // squirrel খেয়ে ফেললো
		}
	}
	for (int i = 0; i < totalGrapes; i++) {
		if (grapes[i].visible && abs(squirrel.x - grapes[i].x) < 50 && abs(squirrel.y - grapes[i].y) < 50) {
			grapes[i].visible = 0;
		}
	}
	for (int i = 0; i < totalApples; i++) {
		if (apples[i].visible && abs(squirrel.x - apples[i].x) < 50 && abs(squirrel.y - apples[i].y) < 50) {
			apples[i].visible = 0;
		}
	}
}

void initializeStrawberries() {
	for (int i = 0; i < MAX_STRAWBERRIES; i++) {
		generateRandomPosition(strawberries[i].x, strawberries[i].y);
		strawberries[i].visible = 1;       // start visible
		strawberries[i].respawnTimer = 0;  // no delay initially
	}
}


// Draw hawk
void drawHawk() {
	if (hawk.visible) {
		if (hawk.direction == 0){

			iShowBMP2(hawk.x, hawk.y, "Images\\Hawk1.bmp", 0xFFFFFF);
		}
		if (hawk.direction == 1){
			iShowBMP2(hawk.x, hawk.y, "Images\\Hawk2.bmp", 0xFFFFFF);
		}
	}
}
void drawSquirrel() {
	if (!squirrel.visible) return;

	if (squirrel.direction == 0) { // moving left → right
		iShowBMP2(squirrel.x, squirrel.y, squirrelRightFrames[squirrelFrameIndex], 0xFFFFFF);
	}
	else if (squirrel.direction == 1) { // moving right → left
		iShowBMP2(squirrel.x, squirrel.y, squirrelLeftFrames[squirrelFrameIndex], 0xFFFFFF);
	}

	// Update animation
	AnimationCounter++;
	if (AnimationCounter >= 5) { // change frame every 5 ticks
		squirrelFrameIndex = (squirrelFrameIndex + 1) % squirrelTotalFrames;
		AnimationCounter = 0;
	}
}


void levelComplete() {
	if (score > highestScore) {
		highestScore = score;
	}
	if (score1 > scoreLevel2) {
		scoreLevel2 = score1;
	}

	// Save scores here as well
	saveScores();

}


void iDraw() {
	iClear();

	if (FirstPage1 == 1){
		First_Page();
	}
	else if (StartPage1 == 1){
		Start_Page();
	}
	else if (StartPage2 == 1){
		DrawStartPage();
	}
	else if (HowToPlayPage == 1) {
		iClear();
		iSetColor(0, 0, 128); // dark blue background
		iShowBMP(0, 0, "Images\\howtoplay.bmp");

		iSetColor(0, 100, 0); // yellow text
		iText(215, 500, "HOW TO PLAY:", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(225, 400, "- Use W/A/S/D or Arrow keys to move", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(225, 350, "- Collect carrots & grapes to score", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(225, 300, "- Apples give lives", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(225, 250, "- Avoid traps!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(225, 200, "- Score 250 to reach Level 2", GLUT_BITMAP_TIMES_ROMAN_24);

		if (isMouseOver(11, 12, 223, 93))
			iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
		else
			iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
		iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (ScoresPage == 1) {
		iClear();
		iSetColor(0, 0, 128); // dark blue background
		iShowBMP(0, 0, "Images\\scores.bmp");
		iSetColor(0, 100, 0);

		//char highScoreStr[50];
		char scoreText[50];
		sprintf_s(scoreText, sizeof(scoreText), "1st Level Score: %d", highestScore);
		iText(300, 420, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf_s(scoreText, sizeof(scoreText), "2nd Level Score: %d", scoreLevel2);
		iText(300, 285, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

		if (isMouseOver(11, 12, 223, 93))
			iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
		else
			iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
		iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (L_FirstPage == 1){
		Level_1FirstPage();
	}
	else if (SecondPage == 1){
		Level_2FirstPage();
	}
	else if (ThirdPage == 1){
		Level_3FirstPage();
	}
	else if (LevelUpPage == 1){
		DrawLevel1Background();
		LevelUpConfirmationPage();
	}
	else if (AboutPage == 1){
		About_Page();
	}
	else if (GameOverPage == 1){
		GameOver_Page();
	}
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my) {
	mouseX = mx;
	mouseY = my;
	//printf("Mouse: %d, %d\n", mouseX, mouseY);
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("x=%d y=%d\n", mx, my);

		// === Hamburger menu on Second Page (StartPage1) ===
		if (StartPage1 == 1 && mx >= 10 && mx <= 40 && my >= 680 && my <= 704) {
			showSettingsMenu = !showSettingsMenu;
		}

		if (StartPage1 == 1 && showSettingsMenu) {
			// Sound toggle
			if (mx >= 70 && mx <= 250 && my >= 680 && my <= 700) {
				soundOn = !soundOn;
				if (soundOn) {
					PlaySound("Audios\\Music.wav", NULL, SND_LOOP | SND_ASYNC);
				}
				else {
					PlaySound(0, 0, 0); // stop music
				}
			}
			// How to Play
			else if (mx >= 70 && mx <= 250 && my >= 650 && my <= 670) {
				StartPage1 = 0;
				HowToPlayPage = 1;
				showSettingsMenu = 0; // hide menu
			}
			// Scores
			else if (mx >= 70 && mx <= 250 && my >= 620 && my <= 640) {
				StartPage1 = 0;
				ScoresPage = 1;
				showSettingsMenu = 0; // hide menu
			}
		}

		if ((FirstPage1 == 1) && (mx >= 342 && mx <= 644) && (my >= 12 && my <= 111))
			PlayButtonClickHandler();
		else if ((StartPage1 == 1) && (mx >= 181 && mx <= 390) && (my >= 273 && my <= 353))
			StartButtonClickHandler();
		else if ((StartPage2 == 1) && (mx >= 402 && mx <= 608) && (my >= 419 && my <= 499))
			Level_1Handle();
		else if ((StartPage2 == 1) && (mx >= 402 && mx <= 610) && (my >= 317 && my <= 398))
			Level_2Handle();
		else if ((StartPage2 == 1) && (mx >= 396 && mx <= 607) && (my >= 215 && my <= 299))
			Level_3Handle();
		else if ((StartPage1 == 1) && (mx >= 182 && mx <= 391) && (my >= 170 && my <= 248))
			AboutPageClickHandler();
		else if ((StartPage2 == 1) && (mx >= 11 && mx <= 223) && (my >= 12 && my <= 93))
			BackButtonClickHandler();
		else if ((L_FirstPage == 1 || SecondPage == 1 || ThirdPage == 1 || GameOverPage == 1) && (mx >= 11 && mx <= 223) && (my >= 12 && my <= 93))
			BackButtonClickHandler2();
		else if ((AboutPage == 1 || ScoresPage == 1 || HowToPlayPage == 1) && (mx >= 11 && mx <= 223) && (my >= 12 && my <= 93))
			BackButtonClickHandler();
		else if (LevelUpPage == 1){
			if ((mx >= 250 && mx <= 400) && (my >= 300 && my <= 350)){
				LevelUpPage = 0;
				SecondPage = 1;
			}
			else if ((mx >= 500 && mx <= 650) && (my >= 300 && my <= 350)){
				LevelUpPage = 0;
				L_FirstPage = 1;
			}
		}
	}
}

void iKeyboard(unsigned char key) {}
void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_F1){
		if (musicon){
			musicon = false;
			PlaySound(0, 0, 0);
		}
		else{
			musicon = true;
			PlaySound("Audios\\Music.wav", NULL, SND_LOOP | SND_ASYNC);

		}
	}

}
void checkFoodCollision() {
	for (int i = 0; i < totalCarrots; i++) {
		if (carrots[i].visible && abs(x - carrots[i].x) < 60 && abs(y - carrots[i].y) < 60) {
			carrots[i].visible = 0;
			carrots[i].respawnTimer = 100;
			if (L_FirstPage == 1) {  // Level 1
				score += 10;
				if (score > highestScore) highestScore = score;
			}
			if (SecondPage == 1) {   // Level 2
				score1 += 10;
				if (score1 > scoreLevel2) scoreLevel2 = score1;
			}
			saveScores();
		}
	}
	for (int i = 0; i < totalGrapes; i++) {
		if (grapes[i].visible && abs(x - grapes[i].x) < 60 && abs(y - grapes[i].y) < 60) {
			grapes[i].visible = 0;
			grapes[i].respawnTimer = 150;
			if (L_FirstPage == 1) {  // Level 1
				score += 8;
				if (score > highestScore) highestScore = score;
			}
			if (SecondPage == 1) {   // Level 2
				score1 += 8;
				if (score1 > scoreLevel2) scoreLevel2 = score1;
			}
			saveScores();
			isHopping = true;
			hopTimer = hopDuration;
		}
	}
	for (int i = 0; i < totalApples; i++) {
		if (apples[i].visible && abs(x - apples[i].x) < 60 && abs(y - apples[i].y) < 60) {
			apples[i].visible = 0;
			apples[i].respawnTimer = 200;
			if (lives < 5){
				lives++;
			}
		}
		// Check collision with traps
		for (int i = 0; i < trapCount; i++) {
			int trapX = traps[i].x;
			int trapY = traps[i].y;
			int trapW = traps[i].width;
			int trapH = traps[i].height;

			// Simple collision check (assuming rabbit size is rabbitWidth x rabbitHeight)
			if (!(x + 95 < trapX || x > trapX + trapW ||
				y + 90 < trapY || y > trapY + trapH)) {
				// Collision happened
				lives--;
				checkGameOver();
				x = 300;  // reset rabbit position
				y = 300;
				break; // optional: stop checking other traps this frame
			}
		}


		if (lives < 0) lives = 0;

		for (int i = 0; i < MAX_STRAWBERRIES; i++) {
			if (strawberries[i].visible && abs(x - strawberries[i].x) < 60 && abs(y - strawberries[i].y) < 60) {
				strawberries[i].visible = 0;
				strawberries[i].respawnTimer = 300; // porerbar auto respawn hobe
				isInvisible = 1;        // rabbit invisible
				invisibleTimer = 300;   // ≈ 9 sec if 30 fps
			}
		}
	}
}


void fixedUpdate() {
	if (L_FirstPage == 1 || SecondPage == 1) {
		bool moved = false;

		int moveStep = 1;
		if (isHopping){
			moveStep = 3;
			hopTimer--;
			if (hopTimer <= 0){
				isHopping = false;
				moveStep = 1;
			}
		}

		if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP)) {
			y += moveStep;
			position = 'w';
			moved = true;
		}
		if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT)) {
			x -= moveStep;
			position = 'a';
			moved = true;
		}
		if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN)) {
			y -= moveStep;
			position = 's';
			moved = true;
		}
		if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
			x += moveStep;
			position = 'd';
			moved = true;
		}


		if (moved) {
			AnimationCounter++;
			if (AnimationCounter >= 5){
				rabbitFrameIndex = (rabbitFrameIndex + 1) % rabbitTotalFrames;
				AnimationCounter = 0;
			}
			checkFoodCollision();
		}
		else{
			rabbitFrameIndex = 0;
		}

		if (x < 0) x = 0;
		if (y < 0) y = 0;

		if (x > screenWidth - rabbitWidth) x = screenWidth - rabbitWidth;
		if (y > screenHeight - rabbitHeight) y = screenHeight - rabbitHeight;

		hawkTimer++;
		if (!hawk.visible && hawkTimer >= hawkSpawnInterval) {
			spawnHawk();
		}
		updateHawk();

		if (score >= 250 && L_FirstPage == 1){
			L_FirstPage = 0;
			LevelUpPage = 1;
			score1 = 0;
		}
	}
	Food* allFoods[] = { carrots, grapes, apples };
	int totals[] = { totalCarrots, totalGrapes, totalApples };

	for (int f = 0; f < 3; f++) {
		for (int i = 0; i < totals[f]; i++) {
			// Only consider invisible foods
			if (!allFoods[f][i].visible && allFoods[f][i].respawnTimer > 0) {
				allFoods[f][i].respawnTimer--;  // countdown by 1 per frame

				// When timer reaches 0, respawn exactly once
				if (allFoods[f][i].respawnTimer == 0) {
					respawnFood(allFoods[f][i]);
				}
			}
		}
	}


	
	if (SecondPage == 1 && score1 >= 250) {
		youWin = 1;   // You Win flag
	}
	if (gameOver) return;
	if (SecondPage == 1) {
		squirrelTimer++;
		if (!squirrel.visible && squirrelTimer >= squirrelSpawnInterval) {
			spawnSquirrel();
		}
		updateSquirrel();
	}
	// Strawberry respawn
	for (int i = 0; i < MAX_STRAWBERRIES; i++) {
		if (!strawberries[i].visible) {
			strawberries[i].respawnTimer--;
			if (strawberries[i].respawnTimer <= 0) {
				generateRandomPosition(strawberries[i].x, strawberries[i].y);
				strawberries[i].visible = 1;
			}
		}
	}

	// Invisibility countdown
	if (isInvisible) {
		invisibleTimer--;
		if (invisibleTimer <= 0) {
			isInvisible = 0;
		}
	}
	//Sleep(10);
}

// Pages
void First_Page() {
	iSetColor(0, 0, 0);
	iShowBMP2(0, 0, "Images\\Rabbit.bmp", 0);
	iShowBMP2(80, 635, "Images\\Name.bmp", 0);

	if (isMouseOver(342, 12, 644, 111))
		iShowBMP2(345, 10, "Images\\play_button2.bmp", 0);
	else
		iShowBMP2(345, 10, "Images\\play_button.bmp", 0);
}

void Start_Page() {
	iShowBMP(0, 0, "Images\\first_page.bmp");

	if (isMouseOver(181, 273, 390, 353))
		iShowBMP2(180, 270, "Images\\Button2.bmp", 0xFFFFFF);
	else
		iShowBMP2(180, 270, "Images\\Button1.bmp", 0xFFFFFF);

	iText(225, 302, "START", GLUT_BITMAP_TIMES_ROMAN_24);

	if (isMouseOver(182, 170, 391, 248))
		iShowBMP2(182, 165, "Images\\Button2.bmp", 0xFFFFFF);
	else
		iShowBMP2(182, 165, "Images\\Button1.bmp", 0xFFFFFF);

	iText(199, 197, "ABOUT US", GLUT_BITMAP_TIMES_ROMAN_24);

	// Settings icon (hamburger menu)
	iSetColor(255, 255, 0);
	iFilledRectangle(2, 670, 50, 50);
	iSetColor(0, 0, 0);
	iFilledRectangle(10, 700, 30, 4);
	iFilledRectangle(10, 690, 30, 4);
	iFilledRectangle(10, 680, 30, 4);


	// If clicked, show menu
	if (showSettingsMenu) {
		iSetColor(255, 255, 0);
		iFilledRectangle(50, 600, 200, 120); // Background box

		iSetColor(0, 0, 0);
		iText(70, 690, soundOn ? "Sound: ON" : "Sound: OFF", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(70, 660, "How to Play", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(70, 630, "Scores", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void DrawStartPage() {
	iFilledRectangle(0, 0, 937, 837);
	iShowBMP(0, 0, "Images\\Doc1.bmp");

	if (isMouseOver(402, 419, 608, 499))
		iShowBMP2(400, 415, "Images\\Button2.bmp", 0xFFFFFF);
	else
		iShowBMP2(400, 415, "Images\\Button1.bmp", 0xFFFFFF);

	if (isMouseOver(402, 317, 610, 398))
		iShowBMP(400, 315, "Images\\Button2.bmp");
	else
		iShowBMP(400, 315, "Images\\Button1.bmp");

	if (isMouseOver(396, 215, 607, 299))
		iShowBMP(400, 215, "Images\\Button2.bmp");
	else
		iShowBMP(400, 215, "Images\\Button1.bmp");

	iText(440, 447, "LEVEL - 1", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(440, 347, "LEVEL - 2", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(440, 247, "LEVEL - 3", GLUT_BITMAP_TIMES_ROMAN_24);

	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);

	

}

void Level_1FirstPage() {
	iShowBMP(0, 0, "Images\\background.bmp");

	// Show rabbit according to direction
	if (position == 'a') {
		iShowBMP2(x, y, rabbitLeftFrames[rabbitFrameIndex], 0xFFFFFF);
	}
	else if (position == 'd') {
		iShowBMP2(x, y, rabbitRightFrames[rabbitFrameIndex], 0xFFFFFF);
	}
	else if (position == 'w') {
		iShowBMP2(x, y, rabbitBackFrames[rabbitFrameIndex], 0xFFFFFF);
	}
	else if (position == 's') {
		iShowBMP2(x, y, rabbitFrontFrames[rabbitFrameIndex], 0xFFFFFF);
	}

	for (int i = 0; i < totalCarrots; i++) {
		if (carrots[i].visible)
			iShowBMP2(carrots[i].x, carrots[i].y, carrots[i].imagePath, 0xFFFFFF);
	}
	for (int i = 0; i < totalGrapes; i++) {
		if (grapes[i].visible)
			iShowBMP2(grapes[i].x, grapes[i].y, grapes[i].imagePath, 0xFFFFFF);
	}
	for (int i = 0; i < totalApples; i++) {
		if (apples[i].visible)
			iShowBMP2(apples[i].x, apples[i].y, apples[i].imagePath, 0xFFFFFF);
	}

	for (int i = 0; i < trapCount; i++) {
		iShowBMP2(traps[i].x, traps[i].y, "Images\\trap.bmp", 0xFFFFFF);
	}


	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);

	iShowBMP(821, 650, "Images\\ScoreDisplay.bmp");
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);
	iSetColor(162, 42, 42);
	iText(842, 695, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

	// Show Lives (Heart icons)
	for (int i = 0; i < lives; i++) {
		iShowBMP2(20 + i * 40, 700, "Images\\life.bmp", 0xFFFFFF);
	}
	drawHawk();
	if (gameOver) {
		GameOver_Page();
		return; // Skip all other drawing
	}

}

void Level_2FirstPage() {
	iShowBMP(0, 0, "Images\\Background2.bmp");

	if (!isInvisible) {
		if (position == 'a')
			iShowBMP2(x, y, rabbitLeftFrames[rabbitFrameIndex], 0xFFFFFF);
		else if (position == 'd')
			iShowBMP2(x, y, rabbitRightFrames[rabbitFrameIndex], 0xFFFFFF);
		else if (position == 'w')
			iShowBMP2(x, y, rabbitBackFrames[rabbitFrameIndex], 0xFFFFFF);
		else if (position == 's')
			iShowBMP2(x, y, rabbitFrontFrames[rabbitFrameIndex], 0xFFFFFF);
	}

	for (int i = 0; i < totalCarrots; i++) {
		if (carrots[i].visible)
			iShowBMP2(carrots[i].x, carrots[i].y, carrots[i].imagePath, 0xFFFFFF);
	}
	for (int i = 0; i < totalGrapes; i++) {
		if (grapes[i].visible)
			iShowBMP2(grapes[i].x, grapes[i].y, grapes[i].imagePath, 0xFFFFFF);
	}
	for (int i = 0; i < totalApples; i++) {
		if (apples[i].visible)
			iShowBMP2(apples[i].x, apples[i].y, apples[i].imagePath, 0xFFFFFF);
	}

	for (int i = 0; i < trapCount; i++) {
		iShowBMP2(traps[i].x, traps[i].y, "Images\\trap.bmp", 0xFFFFFF);
	}

	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);

	iShowBMP(821, 650, "Images\\ScoreDisplay.bmp");
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score1);
	iSetColor(162, 42, 42);
	iText(842, 695, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

	// Show Lives (Heart icons)
	for (int i = 0; i < lives; i++) {
		iShowBMP2(20 + i * 40, 700, "Images\\life.bmp", 0xFFFFFF);
	}
	for (int i = 0; i < MAX_STRAWBERRIES; i++) {
		if (strawberries[i].visible) {
			iShowBMP2(strawberries[i].x, strawberries[i].y, "images\\Strawberry.bmp", 0xFFFFFF);
		}

		drawHawk();
		if (youWin) {
			iShowBMP(0, 0, "Images\\You_Win.bmp");
			//GameOver_Page();
			iSetColor(0, 0, 0);
			return; // Skip drawing the level
		}
		if (gameOver) {
			GameOver_Page();
			return; // Skip all other drawing
		}
		drawSquirrel();
	}
}

void Level_3FirstPage() {
	iFilledRectangle(0, 0, 800, 600);
	iText(300, 300, "Level 3 Coming Soon", GLUT_BITMAP_TIMES_ROMAN_24);
	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}

void About_Page() {
	iShowBMP(0, 0, "Images\\About_Page.bmp");
	iText(350, 700, "Created by Team Hoppy", GLUT_BITMAP_TIMES_ROMAN_24);
	if (isMouseOver(11, 12, 223, 93))
		iShowBMP2(10, 10, "Images\\Button 2.0.bmp", 0xFFFFFF);
	else
		iShowBMP2(10, 10, "Images\\Button1_Reverse.bmp", 0xFFFFFF);
	iText(90, 42, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}

void LevelUpConfirmationPage(){
	
	iShowBMP(300, 350, "Images\\message.bmp");

	iSetColor(0, 0, 0);
	iText(401, 581, "Congratulations!", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(351, 541, "You have scored 250 points.", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(352, 496, "Do you want to go to Level 2?", GLUT_BITMAP_TIMES_ROMAN_24);

	if (isMouseOver(247, 303, 460, 383))
		iShowBMP2(247, 303, "Images\\Button2.bmp", 0xFFFFFF);
	else
		iShowBMP2(247, 303, "Images\\Button1.bmp", 0xFFFFFF);
	iText(286, 339, "YES", GLUT_BITMAP_TIMES_ROMAN_24);

	if (isMouseOver(502, 303, 707, 389))
		iShowBMP2(502, 303, "Images\\Button2.bmp", 0xFFFFFF);
	else
		iShowBMP2(502, 303, "Images\\Button1.bmp", 0xFFFFFF);
	iText(570, 339, "NO", GLUT_BITMAP_TIMES_ROMAN_24);
}

void DrawLevel1Background() {
	iShowBMP(0, 0, "Images\\background.bmp");

	iShowBMP(821, 650, "Images\\ScoreDisplay.bmp");
	char scoreText[50];
	sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);
	iSetColor(162, 42, 42);
	iText(842, 695, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

	for (int i = 0; i < lives; i++) {
		iShowBMP2(20 + i * 40, 700, "Images\\life.bmp", 0xFFFFFF);
	}
}

void generateRandomPosition(int &rx, int &ry){
	rx = rand() % 900;
	ry = rand() % 700;
}

// Function to save scores to a file
void saveScores() {
	_mkdir("Scores");
	FILE *fp = fopen("Scores\\scores.txt", "w"); // text mode
	if (fp == NULL) return;

	// Write scores as text
	fprintf(fp, "%d\n", highestScore);
	fprintf(fp, "%d\n", scoreLevel2);

	fclose(fp);
}

void loadScores() {
	FILE *fp = fopen("Scores\\scores.txt", "r"); // read text
	if (fp == NULL) {
		highestScore = 0;
		scoreLevel2 = 0;
		return;
	}

	// Read scores from text
	fscanf(fp, "%d", &highestScore);
	fscanf(fp, "%d", &scoreLevel2);

	fclose(fp);
}


// Button handlers
void PlayButtonClickHandler() {
	FirstPage1 = 0;
	StartPage1 = 1;
}
void StartButtonClickHandler() {
	StartPage1 = 0;
	StartPage2 = 1;
}
void Level_1Handle() {
	StartPage2 = 0;
	L_FirstPage = 1;
}
void Level_2Handle() {
	StartPage2 = 0;
	SecondPage = 1;
}
void Level_3Handle() {
	StartPage2 = 0;
	ThirdPage = 1;
}
void BackButtonClickHandler() {
	StartPage2 = 0;
	StartPage1 = 1;
	AboutPage = 0;
	ScoresPage = 0;
	HowToPlayPage = 0;
}
void BackButtonClickHandler2() {
	L_FirstPage = 0;
	SecondPage = 0;
	ThirdPage = 0;
	GameOverPage = 0;
	StartPage2 = 1;
	gameOver = 0;
	lives = 5;
	score = 0;
	x = 300;
	y = 300;
}
void AboutPageClickHandler() {
	StartPage1 = 0;
	AboutPage = 1;
}

int main() {
	srand(time(0));
	loadScores();
	for (int i = 0; i < totalCarrots; i++) carrots[i].respawnTimer = 0;
	for (int i = 0; i < totalGrapes; i++) grapes[i].respawnTimer = 0;
	for (int i = 0; i < totalApples; i++) apples[i].respawnTimer = 0;
	initHawk();
	initSquirrel();
	if (musicon){
		PlaySound("Audios\\Music.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	initializeStrawberries();
	iInitialize(966, 743, "Hoppy's Game");
	iSetTimer(30, fixedUpdate); // Animation speed set here
	iStart();
	return 0;
}


