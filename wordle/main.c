#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#ifdef WIN_COMP
#define ClearScreen() system("cls")
#endif

#ifdef LIN_COMP 
#define ClearScreen() system("clear")
#endif



#define INRIGHT 2
#define INWORD  1
#define NOTINWORD 0
void printstate(char *state[6],uint8_t charstate[6][5]);
int main(){

	uint8_t charstate[6][5];
	char *attempts[6];
	char *inputbuf = malloc(100);
	char correctword[6];
	ClearScreen();
	//inserting the correct answer
	while (1){
		printf("Insert a word to guess for another player\n");
		fgets(inputbuf,100,stdin);
		sscanf(inputbuf," %s ",inputbuf);
		if(strlen(inputbuf) != 5){
			printf("Invalid word.\n");
		}else break;
	}
	strcpy(correctword,inputbuf);
	
	ClearScreen();
	//loop in case of another try on the same word
	while (1){
		ClearScreen();
		int inword = 0;
		//resetting the state of the list for every attempt
		for (int i=0;i<6;i++) {attempts[i]=malloc(6);*attempts[i]=0;}
		printstate(attempts,charstate);
		for (int i=0;i<6;i++){
			//inserting and validating guesses
			int winning = 1;
			printf("#%d Insert a word: ",i);
			fgets(inputbuf,100,stdin);
			sscanf(inputbuf," %8s ",attempts[i]);
			if (strlen(attempts[i]) != 5){
				printf("\nThis is not 5 letters in length.\n");
				i--;
			}else{
				//checking whether a letter is inthe right spot, just in word, or doesnt apear in it at all
				for (int j=0;j<5;j++){
					if (tolower(correctword[j]) == tolower(attempts[i][j])){
						charstate[i][j] = INRIGHT;
					}else{
						winning = 0;
						inword = 0; 
						for (int k=0;k<5;k++){
						   if (correctword[k] == attempts[i][j]){
						   charstate[i][j] = INWORD;
						   inword = 1;
						   continue;
							}
						}
						if(!inword) charstate[i][j] = NOTINWORD;	
					}
				}
				ClearScreen();
				printstate(attempts,charstate);
				//case of winning and terminating the program
				if (winning){
				printf("You have won, the answer is %s.",correctword);
				return 0;
				}
			}
		}
		printf("\nYou have lost. Press ENTER to try again with the same word\n");
		fgets(inputbuf,100,stdin);
	}
	return 0;
}
//showing the current state of the game
void printstate(char *state[6],uint8_t charstate[6][5]){
	printf("\n X - Does not apear in the word.\n ? - Apears in the word but not in this place.\n V - It's in the right spot.\n");
	for (int i=0;i<6;i++){
		if(state[i][0]!=0){
			printf("\n _____________\n");
			for (int j=0;j<5;j++)printf(" %c ",toupper(state[i][j]));
			printf("\n");
			for (int j=0;j<5;j++) printf(" %c ",(charstate[i][j]==2)?'V':(charstate[i][j])?'?':'X');
			printf("\n ------------- ");
		}else{
			printf("\n _____________\n #  #  #  #  #\n -------------");
		}
	}
	printf("\n");
}
