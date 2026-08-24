#include<stdio.h>
#include"asciiengine.c"
#include<stdlib.h> 
#include<string.h>
#include<ctype.h>

#define WIDTH 44
#define HEIGHT 25

int main(){
	char* inputbuf = malloc(100);
	uint8_t CurrP = 0;
	CharGrid* P1 = InitCharGrid((V2i){10,10},'~');
	CharGrid* P2 = InitCharGrid((V2i){10,10},'~');
	CharGrid* MainScreen = InitCharGrid((V2i){WIDTH,HEIGHT},' ');
	char* bottomline = MainScreen->data+IDX(0,HEIGHT-1,WIDTH);
	
	uint8_t ships[2][4] = {{2,3,3,4},{2,3,3,4}};
	//board notations
	for (int i = 0;i<10;i++) {
	SetGridChar(65+i,(V2i){0,i+2},MainScreen);
	SetGridChar(65+i,(V2i){0,14+i},MainScreen);
	SetGridChar(48+i,(V2i){1+i,1},MainScreen);
	SetGridChar(48+i,(V2i){1+i,13},MainScreen);}

	//texts
	memcpy(MainScreen->data+IDX(0,0,WIDTH),"enemy board",11);	
	memcpy(MainScreen->data+IDX(0,12,WIDTH)," your board",11);	
	memcpy(MainScreen->data+IDX(14,0,WIDTH),"P1's turn",9);	
	memcpy(MainScreen->data+IDX(14,2,WIDTH),"ships:",6);	
	//drawing available ships to place
	for(int i = 0;i<4;i++){
		SetGridChar(48+i,(V2i){14,4+i*2},MainScreen);
		memset(MainScreen->data+IDX(16,4+i*2,WIDTH),'@',ships[0][i]);
	} 
	DrawCharRect((V2i){1,2},(V2i){10,11},'?',MainScreen);

	//Setup Stage	
	int Xpos,Ypos,shipnum,curshiplen,overlap;
	char dir,dirinp,Yposinp;
	shipnum = 0;
	//---start of setup---
	while (CurrP < 2){
		CharGrid* CurrB = (CurrP==0)?P1:P2;
		//CleanCLI();
		printf("\n%d\n",shipnum);
		//preparing screen for every turn
		DrawCharRect((V2i){14,4},(V2i){19,10},' ',MainScreen);
		for(int i = 0;i<4;i++){
		SetGridChar(48+i,(V2i){14,4+i*2},MainScreen);
		memset(MainScreen->data+IDX(16,4+i*2,WIDTH),'@',ships[CurrP][i]);}
		DrawCharGrid((CurrP==0)?P1:P2,MainScreen,(V2i){1,14});
		memcpy(MainScreen->data+IDX(14,0,WIDTH),(CurrP==0)?"P1's turn":"P2's turn",9);
		//CleanCLI();	
		ShowScreen(MainScreen);

		//in case all ships have been placed
		if(shipnum == 4){
			uint8_t cont = 0;
			while(1){
				printf("is this configuration of ship right? Y/N");
				fgets(inputbuf,100,stdin);
				sscanf(inputbuf,"%c",inputbuf);
				if(toupper(inputbuf[0])=='Y'){
					CurrP++;
					CleanCLI();
					if(CurrP==1)printf("P1 has set their ships, press enter for P2 to set their ships");
					shipnum = 0;
					cont++;
					break;
				}else if(toupper(inputbuf[0])=='N'){
					shipnum = 0;
					memset(CurrB->data,'~',100);
					ships[CurrP][0]=2;ships[CurrP][0]=2;ships[CurrP][1]=3;ships[CurrP][2]=3;ships[CurrP][3]=4;
					cont++;
					break;
				}
				
			}
			if (cont) continue;	
		}

		//reading input and verifying
		printf("Pick a position to place A0-J9 and directon u/d/l/r: ");
		fgets(inputbuf,100,stdin);
		if (sscanf(inputbuf,"%c%d %c",&Yposinp,&Xpos,&dirinp)!=3||
		!(INBOUNDS('A','J',toupper(Yposinp)))||!(INBOUNDS(0,9,Xpos))||
		!(toupper(dirinp)=='U'||toupper(dirinp)=='D'||toupper(dirinp)=='L'||toupper(dirinp)=='R')){
			sprintf(bottomline,"invalid input, try again");
			continue;
		}

		//setting helper variables and adjusting
		Ypos = (uint8_t)toupper((uint8_t)(Yposinp))-65;
		curshiplen = ships[CurrP][shipnum];
		dir = toupper((uint8_t)dirinp);		
		overlap = 0;
		V2i vecdir = (V2i){
			(dir=='L')?-1:(dir=='R')?1:0,
			(dir=='U')?-1:(dir=='D')?1:0 
		};
		V2i vecpos = (V2i){Xpos,Ypos};

		if(ChKGridPos(AddV2i(vecpos,ScaleV2i(vecdir,curshiplen)),CurrB)){
			//checking for intersecting ships
			printf("|}%d %d|",Ypos,curshiplen);
			for(int i = 0;i<curshiplen;i++){
				printf("works");
				printf("\n|%c|\n",GetGridChar(AddV2i(vecpos,ScaleV2i(vecdir,i)),CurrB));
				if(GetGridChar(AddV2i(vecpos,ScaleV2i(vecdir,i)),CurrB)=='@'){
				overlap++;break;};
			}
			printf("overlap:%d",overlap);
			if(!(overlap)){
				//drawing the ship on the board and moving to the next one in case of success
				DrawCharRect(vecpos,AddV2i(vecpos,ScaleV2i(vecdir,curshiplen-1)),'@',(CurrP==0)?P1:P2);
				ships[CurrP][shipnum]=0;shipnum++;
				memset(bottomline,' ',WIDTH);
				continue;
			//error messages
			}else{sprintf(bottomline,"your ship colides with others");continue;}
		}else{sprintf(bottomline,"your ship reaches out of bounds");continue;}
	}
	
	//CleanCLI();
	printf("The ships have been placed. the game will now beign. press ENTER for P1 to start \n P2 should look away from the monitor durnig P1's turns and viceversa.");
	fgets(inputbuf,100,stdin);
	//---end of setup---
	
	//preparation for the start of the game	
	//wiping the list of ships	
	DrawCharRect((V2i){12,3},(V2i){19,10},' ',MainScreen);
	CurrP = 0;
	int shipchars[2];
	shipchars[0] = 12;shipchars[1] = 12;
	CharGrid* hitprv = InitCharGrid((V2i){10,10},'0');	

	//---Start---	
	while(shipchars[0] != 0 && shipchars[1] != 0){
		//settint enemy Board pointer
		CharGrid* EnemyB = (CurrP)?P1:P2;
		memcpy(MainScreen->data+IDX(14,0,WIDTH),(CurrP==0)?"P1's turn":"P2's turn",9);	

		//preparing the enemy's board with current player's shots
		memset(hitprv->data,'~',100);
		for (int i =0 ;i<10;i++)for(int j =0;j<10;j++){
			char prvbuf = GetGridChar((V2i){j,i},EnemyB);
			if (prvbuf == 'X' || prvbuf == 'O') SetGridChar(prvbuf,(V2i){j,i},hitprv);}
		DrawCharGrid(hitprv,MainScreen,(V2i){1,2});

		DrawCharGrid((!(CurrP))?P1:P2,MainScreen,(V2i){1,14});
		//revealing the screen for shooting
		//CleanCLI();
		ShowScreen(MainScreen);
		memset(bottomline,' ',WIDTH);

		//taking input for shot
		printf("Pick a position to shoot A0-J9: ");
		fgets(inputbuf,100,stdin);
		if (sscanf(inputbuf,"%c%d",&Yposinp,&Xpos)!=2||
		!(INBOUNDS('A','J',toupper(Yposinp)))||!(INBOUNDS(0,9,Xpos))){
			sprintf(bottomline,"invalid input, try again");
			continue;
		}
		
		Ypos = (uint8_t)toupper((uint8_t)(Yposinp))-65;
		V2i vecpos = (V2i){Xpos,Ypos};
		//in case of miss/hit/repeat
		unsigned char hitresult = GetGridChar(vecpos,EnemyB);
		if (hitresult == '~'){
			SetGridChar('O',vecpos,EnemyB);
			sprintf(bottomline,"You missed.");	
		}else if(hitresult == 'O' || hitresult == 'X'){
			sprintf(bottomline,"you already hit that");
			continue;
		}else if(hitresult == '@'){
			sprintf(bottomline,"you hit enemy's ship");	
			SetGridChar('X',vecpos,EnemyB);
			shipchars[!(CurrP)]--;
		}
		
		//preparing the board of current players shot after the shot
		for (int i =0 ;i<10;i++)for(int j =0;j<10;j++){
			char prvbuf = GetGridChar((V2i){j,i},EnemyB);
			if (prvbuf == 'X' || prvbuf == 'O') SetGridChar(prvbuf,(V2i){j,i},hitprv);}
		DrawCharGrid(hitprv,MainScreen,(V2i){1,2});

		//updating the shot status
		CleanCLI();
		ShowScreen(MainScreen);
		memset(bottomline,' ',WIDTH);
		
		if (shipchars[0] == 0){
			printf("P2 has won.");
			return 0;
		}else if (shipchars[1] == 0){
			printf("P1 has won.");
			return 0;
		}

		//switching to the other player	
		fgets(inputbuf,100,stdin);
		CleanCLI();
		printf("press ENTER to show P%d's board.",(!(CurrP)+1));	
		fgets(inputbuf,100,stdin);
		CurrP = !(CurrP);
	}
}
