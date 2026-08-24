#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#ifdef _WIN32
#include<windows.h>
#define CleanCLI() system("cls") 
#define WaitMs(t) Sleep(t)
#endif

#ifdef __linux__
#include<unistd.h>
#define CleanCLI() system("clear") 
#define WaitMs(t) usleep(t*1000)

#endif

int main(int argc, char ** argv){
	int taskcount = 10;
	int minval = 1;
	int maxval = 10;
	int seed = 0;
	for(int i = 1;i<argc;i++){
		sscanf(argv[i],"min:%d",&minval);
		sscanf(argv[i],"max:%d",&maxval);
		sscanf(argv[i],"count:%d",&taskcount);
		sscanf(argv[i],"seed:%d",&seed);
		if (strcmp(argv[i],"help")==0){
			printf("Available flags for this program.\n");
			printf("max:(val) - maximal value of a factor\n");
			printf("min:(val) - minimal value of a factor\n");
			printf("count:(val) - amount of task the program will output\n");
			printf("seed:(val) - seed for the randomized factors, the values outputted with the same seed on different devices/operating systems may vary. inserting 0 will result in a randomized sequence.\n");
			exit(0);
		}
	}

	if (maxval == minval){
		printf("The maximum value and minimal value are the same. Choose different values.");
		exit(0);
	}else if(minval > maxval){
		printf("The minimum value is greather than maximum. Choose different values.");
		exit(0);
	}else if(taskcount < 1){
		printf("The amount of problems should be larger than 0. Choose a different value.");
		exit(0);
	}
		
	time_t timehelp;
	time(&timehelp);
	if(seed == 0){
		srand(timehelp);
	}else srand(seed); 
	rand();
	int val1,val2,ans;
	int points = 0;
	char inputbuffer[256];
	for (int i = 0;i<taskcount;i++){
		val1 = minval + rand()%(maxval - minval + 1);	
		val2 = minval + rand()%(maxval - minval + 1);	
		printf("Solve:\n%d * %d\n",val1,val2);
		while(1){
			fgets(inputbuffer,256,stdin); 
			if (sscanf(inputbuffer,"%d",&ans)) break; 
			printf("Unable to read vlaue.\n");
		}
		if (ans == val1*val2){
			printf("Correct\n");
			points++;	
		}else printf("Incorrect\n");	
	}

	printf("Your score: %d/%d in %d seconds\n",points,taskcount,time(NULL)- timehelp);
	return 0;
}
