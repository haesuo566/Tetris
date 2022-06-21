#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "wincon.h"

#define width 13
#define height 20
#define maxblock (width*height)
#define blocksize 4
#define maginex 6

int fBuf[height][width];
int bBuf[height][width];

typedef struct block1 {
	int x;
	int y;
}*block;

struct block1 blocks[blocksize];
block stackedBlock[maxblock];
int gameloop = 0;
int rand1 = 0;
int rand2 = 0;
int x = 0;
int y = 0;

struct block1 blockPos[5][4][4] = {
	{{0,-1,1,-1,-1,-1,0,-2},{0,0,1,-1,0,-1,0,-2},{0,0,1,-1,-1,-1,0,-1},{0,0,0,-1,-1,-1,0,-2}},
	{{0,0,0,-1,-1,-1,-1,-2},{0,-1,1,-2,-1,-1,0,-2},{1,0,1,-1,0,-1,0,-2},{0,0,1,-1,-1,0,0,-1}},
	{{0,1,0,0,0,-1,0,-2},{0,0,2,0,-1,0,1,0},{0,2,0,1,0,0,0,-1},{0,0,1,0,-2,0,-1,0}},
	{{0,0,1,0,0,-1,0,-2},{-1,0,1,-1,-1,-1,0,-1},{0,0,0,-1,-1,-2,0,-2},{0,-1,1,-1,-1,-1,1,-2}},
	{{0,0,0,-1,-1,0,-1,-1},{0,0,0,-1,-1,0,-1,-1},{0,0,0,-1,-1,0,-1,-1},{0,0,0,-1,-1,0,-1,-1}}
};

void initialize();
void rendering();
void setFrame();
void pressKey();
void drawBlock();
void moveBlock();
void createBlock();
void deleteBlock();
void spinBlock();
void endGame();
int isClash();

int main() {
	initialize();
	
	createBlock();
	
	while(!gameloop) {
		drawBlock();
		rendering();
		moveBlock();
		pressKey();
		Sleep(200);
	}
	
	int i;
	for(i = 0; i < maxblock; i++) {
		if(stackedBlock[i] != NULL) {
			free(stackedBlock[i]);
		}
	}
	
	while(1) {
		system("pause");
	}
	
	return 0;
}

void deleteBlock() {
	int i = height - 1, j, check = 1;
	while(i) {
		int cnt = 0;
		int buffer[width] = {0,};
		
		for(j = 0; j < maxblock; j++) {
			if(stackedBlock[j] != NULL && stackedBlock[j]->y == i) {
				buffer[cnt] = j;
				cnt++;
			}
		} 
		
		if(cnt >= width) {
			for(j = 0; j < width; j++) {
				free(stackedBlock[buffer[j]]);
				stackedBlock[buffer[j]] = NULL;
			}
			
			for(j = 0; j < maxblock; j++) {
				if(stackedBlock[j] != NULL && stackedBlock[j]->y < height - 1 && stackedBlock[j]->y < i) {
					(stackedBlock[j]->y)++;
				}
			}//for
			
			i = height;
		}//if
		
		i--;
	}//while
}

void endGame() {
	int i;
	for(i = 0; i < maxblock; i++) {
		if(stackedBlock[i] != NULL && stackedBlock[i]->y <= 0) {
			 gameloop = 1;
		}
	}
}

int isClash() {
	int i;
	for(i = 0; i < maxblock; i++) {
		if(stackedBlock[i] != NULL) {
			
			int j;	
			for(j = blocksize - 1; j >= 0; j--) {
				if(stackedBlock[i]->y - 1 == blocks[j].y) {
					if(stackedBlock[i]->x + 1 == blocks[j].x ||
						stackedBlock[i]->x - 1 == blocks[j].x) {
						return 1;
					}
				}
			}
		}//if
	}//for
	
	return 0;
}

void moveBlock() {
	int i;
	for(i = 0; i < maxblock; i++) {
		if(stackedBlock[i] != NULL) {
			
			int j;	
			for(j = blocksize - 1; j >= 0; j--) {
				if(stackedBlock[i]->x == blocks[j].x &&
					stackedBlock[i]->y - 1 == blocks[j].y) {
					int n = 0;
					
					for(i = 0; i < maxblock; i++) {
						if(n < 4) {
							if(stackedBlock[i] == NULL) {
								stackedBlock[i] = (block)malloc(sizeof(block));
								
								stackedBlock[i]->x = blocks[n].x;
								stackedBlock[i]->y = blocks[n].y;
								n++;
							}
						}
						else {
							break;
						}
					}//for
					endGame();
					deleteBlock();
					createBlock();
					return;
				}//if
			}//for
		}//if
	}//for

	if(blocks[0].y == height - 1) {
		int n = 0;
		for(i = 0; i < maxblock; i++) {
			if(n < 4) {
				if(stackedBlock[i] == NULL) {
					stackedBlock[i] = (block)malloc(sizeof(block));
					
					stackedBlock[i]->x = blocks[n].x;
					stackedBlock[i]->y = blocks[n].y;
					n++;
				}
			}
			else {
				break;
			}
		}//for
		deleteBlock();
		createBlock();
		return;
	}//if
		
	y++;
	for(i = blocksize - 1; i >= 0; i--) {
		(blocks[i].y)++;
	}
}

void createBlock() {
	rand1 = rand() % 5;
	rand2 = rand() % 4;
	int len = rand() % width;
	
	int i;
	for(i = 0; i < blocksize; i++) {
		blocks[i].x = blockPos[rand1][rand2][i].x + len;
		blocks[i].y = blockPos[rand1][rand2][i].y;
	}
	
	y = 0;
	x = len;
	
	while(blocks[2].x < 0) {
		for(i = 0; i < blocksize; i++) {
			(blocks[i].x)++;
		}
	}
	
	while(blocks[1].x > width - 1) {
		for(i = 0; i < blocksize; i++) {
			(blocks[i].x)--;
		}
	}
}

void pressKey() {
	int i;
	if(GetAsyncKeyState(VK_DOWN)) {
		moveBlock();
	}
	if(GetAsyncKeyState(VK_LEFT)) {
		if(blocks[2].x == 0) {
			return;
		}
		
		if(isClash()) {
			return;
		}
		
		x--;
		for(i = blocksize - 1; i >= 0 ; i--) {
			(blocks[i].x)--;
		}
	}//if
	if(GetAsyncKeyState(VK_RIGHT)) {
		if(blocks[1].x == width - 1) {
			return;
		}
		
		if(isClash()) {
			return;
		}
		
		x++;
		for(i = blocksize - 1; i >= 0 ; i--) {
			(blocks[i].x)++;
		}
	}//if
	//약간 수정. 
	if(GetAsyncKeyState(VK_SPACE)) {
		rand2++;
		
		if(rand2 > 3) {
			rand2 = 0;
		}
		
		if(blockPos[rand1][rand2][1].x + x > width - 1 ||
			blockPos[rand1][rand2][2].x + x < 0) {
			return;
		}
		
		for(i = 0; i < blocksize; i++) {
			blocks[i].x = blockPos[rand1][rand2][i].x + x;
			blocks[i].y = blockPos[rand1][rand2][i].y + y;
		}
	}//if
}

void drawBlock() {
	int i;
	for(i = 0; i < blocksize; i++) {
		bBuf[blocks[i].y][blocks[i].x] = 1;
	}
	
	for(i = 0; i < maxblock; i++) {
		if(stackedBlock[i] != NULL) {
			bBuf[stackedBlock[i]->y][stackedBlock[i]->x] = 1;
		}
	}
}

void initialize() {
	hideCursor();
	system("mode con cols=50 lines=25");
	
	int i, j;
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			fBuf[j][i] = 0;
			bBuf[j][i] = 0; 
		}
	}
	
	for(i = 0; i < maxblock; i++) {
		stackedBlock[i] = NULL;
	}
	
	srand((unsigned int)time(NULL));
	setFrame();
}

void rendering() {
	int i, j;
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			
			if(fBuf[j][i] != bBuf[j][i]) {
				setLocation((i * 2) + maginex, j);
				
				if(bBuf[j][i] == 0) {
					printf("  ");
				}
				else if(bBuf[j][i] == 1) {
					printf("%s", "■");
				}
			}
		}
	}
	
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			fBuf[j][i] = bBuf[j][i];
			bBuf[j][i] = 0; 
		}
	}
}

void setFrame() {
	int i, j;
	for(i = 0; i < width + 2; i++) {
		setLocation((i * 2) + maginex - 2, height);
		printf("%s", "▩");
	}
	
	for(j = 0; j < height; j++) {
		for(i = 0; i < (width + 2); i++) {
			setLocation((i * 2) + maginex - 2, j);
			
			if(i == 0) {
				printf("%s", "▩");
			}
			else if(i == width + 1) {
				printf("%s", "▩");	
			}
		}
	}
}
