#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include"vector.c"

#ifdef _WIN32
#include<windows.h>
#define CleanCLI() system("cls") 
#define WaitM(t) Sleep(t)
#endif

#ifdef __linux__
#include<unistd.h>
#define CleanCLI() system("clear") 
#define WaitM(t) usleep(t*1000)

#endif

#define MAX(a,b) (((a)<(b)) ? (b):(a))
#define MIN(a,b) (((a)>(b)) ? (b):(a))
#define MINMAX(a,b,c) MAX((a),MIN((b),(c)))
#define INBOUNDS(a,b,c) MINMAX(a,b,c) == (c)

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 16
#define IDX(xp,yp,xm)  (xp) + ((xm) * (yp))

typedef struct{
	unsigned char ignore; 
	V2i size;
	unsigned char* data;
}CharGrid;
//definig structs

CharGrid* InitCharGrid(V2i size,unsigned char character);
void printSprite(CharGrid *src,CharGrid* grid);
void ShowScreen(CharGrid* grid);
void DrawCharGrid(CharGrid* sprite,CharGrid* grid,V2i pos);
CharGrid* LoadCharGrid(char path[]);
uint8_t SetGridChar(unsigned char character ,V2i pos,CharGrid* grid);
uint8_t ChKGridPos(V2i pos,CharGrid* grid);
void DrawCharRect(V2i p1, V2i p2, unsigned char character, CharGrid* grid);

//declaring functions
uint8_t SetGridChar(unsigned char character,V2i pos,CharGrid* grid){ 
	if(INBOUNDS(0,grid->size.x-1,pos.x) && INBOUNDS(0,grid->size.y-1,pos.y)) {grid->data[IDX(pos.x,pos.y,grid->size.x)] = character; return 1;}else{return 0;}
}

void DrawCharRect(V2i p1, V2i p2, unsigned char character, CharGrid* grid){
	V2i pos = (V2i){MIN(p1.x,p2.x),MIN(p1.y,p2.y)};	
	V2i siz = (V2i){MAX(p1.x,p2.x),MAX(p1.y,p2.y)};
	for (int i = pos.y;i<=siz.y;i++){
		for (int j = pos.x;j<=siz.x;j++){
			SetGridChar(character,(V2i){j,i},grid);
		}
	}

}

uint8_t ChKGridPos(V2i pos,CharGrid* grid){
	return (INBOUNDS(0,grid->size.x-1,pos.x) && INBOUNDS(0,grid->size.y-1,pos.y));
}

void ShowScreen(CharGrid* grid){
	putchar('\n');
	for (int i = 0;i < grid->size.y;i++){
		for(int j = 0;j < grid->size.x;j++){
			printf("%c",grid->data[IDX(j,i,grid->size.x)]);
		}
		printf("\n");
	}
	return ;
}

unsigned char GetGridChar(V2i pos,CharGrid* grid){
	unsigned char result = (
		INBOUNDS(0,grid->size.x-1,pos.x) && 
		INBOUNDS(0,grid->size.y-1,pos.y))?
		grid->data[IDX(pos.x,pos.y,grid->size.x)]:0;
	return result;
}

CharGrid* InitCharGrid(V2i size,unsigned char character){
	CharGrid* buffer=malloc(sizeof(CharGrid));
	*buffer = (CharGrid){.size=size,.data=memset(malloc(size.x*size.y),character,size.x*size.y)};
	return buffer;
}

void DrawCharGrid(CharGrid* sprite,CharGrid* grid, V2i pos){
	int X = grid->size.x;
	int Y = grid->size.y;
	int SX =sprite->size.x;
	int SY =sprite->size.y;
	unsigned char ignore=sprite->ignore;
	for(int i=0;i<SX;i++){
		for(int j=0;j<SY;j++){	
			if (INBOUNDS(0,Y-1,i+pos.y) && INBOUNDS(0,X-1,j+pos.x) && sprite->data[IDX(j,i,SX)]!=ignore){
				grid->data[IDX(j+pos.x,i+pos.y,X)] = sprite->data[IDX(j,i,SX)];
			}
		}
	}

	
	
	return;

}
//defining functions

CharGrid* LoadCharGrid(char* path){

	int width;
	int height;
	
	CharGrid* grid = malloc(sizeof(CharGrid));
	FILE *f = fopen(path,"r");
	char buffer;
	char longBuffer[300];
	char ignore;	

	fgets(longBuffer,300,f);
	if (sscanf(longBuffer,"%d %d '%c'",&width,&height,&ignore) != 3){
		printf("bad file, unable to load dimsnsions and ignore character");
		exit(1);
	}
	printf("\nthe ignore char is:'%c'\n",ignore);
	//receiving dimensions from the beginning of the file
	grid->data = malloc(width*height);	
	grid->size = (V2i){width,height};
	grid->ignore = ignore;
	for(int i = 0;i<height;i++){
		for(int j = 0;j<=width;j++){
			buffer = fgetc(f);
			//printf("%c",buffer);
			if((buffer == EOF) || (((j!=width) && (buffer=='\n'))) || ((j==width)&&(buffer!='\n'))){
				printf("%d %d bad file! wrong char:'%c'",i,j,buffer);
				exit(1);
			}else if (j!=width) grid->data[IDX(j,i,width)]=buffer;	
		}
	}
	//putting characters from the file into the sprite struct value
	return grid;
}




